package handlers;

import static java.net.HttpURLConnection.*;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import models.Game;
import models.Round;
import requests.GeneralRequest;
import responses.GetIsChatEnabledResponse;
import responses.GetPlayerShouldPlayResponse;
import server.Server;

public class GetPlayerShouldPlayHandler implements HttpHandler {

	@Override
	public void handle(HttpExchange exchange) throws IOException {
		Scanner in = new Scanner(exchange.getRequestBody());
		String jsonStr = "\n";
		
		while(in.hasNextLine()) {
			String nextLine = in.nextLine();
			jsonStr += nextLine + "\n";
		}

		in.close();
		
		GeneralRequest request = convertFromJson(jsonStr);
		GetPlayerShouldPlayResponse response = new GetPlayerShouldPlayResponse();
		
		String playerID = request.getPlayerID();
		
		Game game = Server.getPlayerCurrentGame(playerID);
		
		if(game.getPlayer1ID().equals(playerID)) {
			response.setPartner(game.getPlayer2ID());
			response.setPlayerShouldPlay(getPlayStatus(game,1));
		}
		else if(game.getPlayer2ID().equals(playerID)) {
			response.setPartner(game.getPlayer1ID());
			response.setPlayerShouldPlay(getPlayStatus(game,2));
		}
		
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
	}
	
	private boolean getPlayStatus(Game game, int playerNum) {
		if(game.getRounds().size()==0) {
			return true;
		}
		else {
			Round round = game.getRounds().get(game.getRounds().size()-1);
			if(playerNum == 1) {
				int choice = round.getPlayer1Choice();
				if(choice == -1) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				int choice = round.getPlayer2Choice();
				if(choice == -1) {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	
	private GeneralRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		GeneralRequest request = new GeneralRequest();
		
		request = gson.fromJson(jsonStr, GeneralRequest.class);
		
		return request;
	}
	
	private String convertToJson(GetPlayerShouldPlayResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}