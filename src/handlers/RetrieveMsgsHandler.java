package handlers;

import static java.net.HttpURLConnection.*;

import java.time.*;
import java.io.FileWriter;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import models.ChatMsg;
import models.Game;
import models.Round;
import requests.GeneralRequest;
import requests.SendMsgRequest;
import responses.GeneralResponse;
import responses.RetrieveMsgsResponse;
import server.Server;

public class RetrieveMsgsHandler implements HttpHandler {

	@Override
	public void handle(HttpExchange exchange) throws IOException {
		
		Scanner in = new Scanner(exchange.getRequestBody());
		String jsonStr = "\n";
		
		while(in.hasNextLine()) {
			String nextLine = in.nextLine();
			jsonStr += nextLine + "\n";
		}

		in.close();
		
		GeneralRequest request = this.convertFromJson(jsonStr);
		RetrieveMsgsResponse response = new RetrieveMsgsResponse();
		String playerID = request.getPlayerID();
		
		//Get last message for response
		Game game = Server.getPlayerCurrentGame(playerID);
		ArrayList<ChatMsg> chatMsgs =  new ArrayList<ChatMsg>();
		ArrayList<ChatMsg> temp = game.getMessages();
		for (int i = temp.size()-1; i >= 0; i--) {
			if (!temp.get(i).getPlayerID().equals(playerID)) {
				chatMsgs.add(temp.get(i));
				break;
			}
		}
		response.setChatMsgs(chatMsgs);
		
		if(response.getChatMsgs() == null) {
			response.setErrorMsg("player not in game");
		}
		
		exchange.sendResponseHeaders(HTTP_OK, 0);

		// 		
		Server.removeMessagedPlayer(playerID);
		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();	
	}
	
	private GeneralRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		GeneralRequest request = new GeneralRequest();
		
		try {
			request = gson.fromJson(jsonStr, GeneralRequest.class);
		} catch (Exception e) {
			request.setErrorMsg("Bad param(s)");
		}
		
		return request;
	}
	
	private String convertToJson(RetrieveMsgsResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}
