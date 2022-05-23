package handlers;

import static java.net.HttpURLConnection.*;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;
import java.util.Scanner;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import models.Game;
import models.ScheduleItem;
import requests.GeneralRequest;
import requests.LoginRequest;
import responses.CheckStatusResponse;
import responses.GeneralResponse;
import server.Server;

public class CheckStatusHandler implements HttpHandler {

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
		CheckStatusResponse response = new CheckStatusResponse();
		
		if(Server.getReadyPlayers().contains(request.getPlayerID())) {
			response.setStatus("waiting for game");
		}
		else if(Server.getDonePlayers().contains(request.getPlayerID())) {
			response.setStatus("done with all games");
		}
		else if(Server.getBusyPlayers().contains(request.getPlayerID())) {
			response.setStatus("playing a game");			
		}
		else if(playerIDInSystem(request.getPlayerID())) {
			response.setStatus("not logged in");
		}
		else {
			response.setStatus("playerID not in system");
		}
		
		if(response.getStatus().equals("playing a game")) {
			response.setPlayerNum(getPlayerNum(request.getPlayerID()));
		}
		
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
		
	}
	
	private int getPlayerNum(String playerID) {
		Game game = Server.getPlayerCurrentGame(playerID);
				
		if(game.getPlayer1ID().equals(playerID)) {
			return 1;
		}
		else if(game.getPlayer2ID().equals(playerID)) {
			return 2;
		}
		return 0;
	}
	
	private boolean playerIDInSystem(String playerID){
		for(ScheduleItem item:Server.getSchedule().getScheduleItems()) {
			if(item.getUserID().equals(playerID)) {
				return true;
			}
		}
		return false;
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
	
	private String convertToJson(CheckStatusResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}