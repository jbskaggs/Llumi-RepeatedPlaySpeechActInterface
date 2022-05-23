package handlers;

import static java.net.HttpURLConnection.*;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import models.ChatMsg;
import models.Game;
import requests.SendMsgRequest;
import responses.GeneralResponse;
import server.Server;

public class SendChatMsgHandler implements HttpHandler {

	@Override
	public void handle(HttpExchange exchange) throws IOException {

		
		Scanner in = new Scanner(exchange.getRequestBody());
		String jsonStr = "\n";
		
		while(in.hasNextLine()) {
			String nextLine = in.nextLine();
			jsonStr += nextLine + "\n";
		}
		System.out.println(jsonStr);

		in.close();
		
		SendMsgRequest request = this.convertFromJson(jsonStr);
		GeneralResponse response = new GeneralResponse();
		boolean msgAdded = false;
		String playerID = request.getPlayerID();
		
		Game game = Server.getPlayerCurrentGame(playerID);
		
		if(game.getPlayer1ID().equals(request.getPlayerID()) || game.getPlayer2ID().equals(request.getPlayerID())) {
			ChatMsg chatMsg = new ChatMsg();
			/*
			chatMsg.setGameID(game.getGameID());
			if(game.getRounds().size()==0) {
				chatMsg.setRoundNum(0);
			}
			else {
				chatMsg.setRoundNum(game.getRoundNum());
			}
			*/
			chatMsg.setPlayerID(request.getPlayerID());
			chatMsg.setMessage(request.getMsg());
			game.addMessage(chatMsg);
			msgAdded = true;
		}
		
		String partner = "";
		if (game.getPlayer1ID().equals(request.getPlayerID())) {
			partner = game.getPlayer2ID();	
		}
		if (game.getPlayer2ID().equals(request.getPlayerID())) {
			partner = game.getPlayer1ID();
		}
				
		if(msgAdded) {
			Server.addMeassgedPlayer(partner);
			response.setMsg(partner);//("message sent");
		}
		else {
			response.setErrorMsg("error in sending message");
		}
			
		exchange.sendResponseHeaders(HTTP_OK, 0);
		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
		
	}
	
	private SendMsgRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		SendMsgRequest loginRequest = new SendMsgRequest();
		
		try {
			loginRequest = gson.fromJson(jsonStr, SendMsgRequest.class);
		} catch (Exception e) {
			loginRequest.setErrorMsg("Bad param(s)");
		}
		
		return loginRequest;
	}
	
	private String convertToJson(GeneralResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}
