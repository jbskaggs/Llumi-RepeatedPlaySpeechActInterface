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
import responses.GetGameResultsResponse;
import responses.GetRoundResultsResponse;
import server.Server;

public class GetGameResultsHandler implements HttpHandler {

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
		GetGameResultsResponse response = null;
		
		String playerID = request.getPlayerID();
		
		Game game = Server.getPlayerCurrentGame(playerID);
		response = createResponse(game,playerID);
	
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
	}
	
	private GetGameResultsResponse createResponse(Game game, String playerID) {
		GetGameResultsResponse response = new GetGameResultsResponse();
		response.setGameID(game.getGameID());
		response.setNumRoundsCompleted(game.getRounds().size());
		response.setNumRoundsTotal(Server.getCollectionOfGamesMatrices().get(game.getGameID()).size());
		
		double sum = 0;
		
		if(game.getPlayer1ID().equals(playerID)) {
			for(Round round:game.getRounds()) {
				if(round.getPlayer1Payoff() != -1) {
					sum += round.getPlayer1Payoff();
				}
			}
		}
		else {
			for(Round round:game.getRounds()) {
				if(round.getPlayer2Payoff() != -1) {
					sum += round.getPlayer2Payoff();
				}
			}		
		}
		response.setSummedPayoff((int)(sum / 2.81));
		
		return response;
	}
	
	private GeneralRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		GeneralRequest request = new GeneralRequest();
		
		request = gson.fromJson(jsonStr, GeneralRequest.class);
		
		return request;
	}
	
	private String convertToJson(GetGameResultsResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}