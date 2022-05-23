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
import responses.GetPlayerShouldPlayResponse;
import responses.GetRoundResultsResponse;
import server.Server;

public class GetRoundResultsHandler implements HttpHandler {

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
		GetRoundResultsResponse response = null;
		
		String playerID = request.getPlayerID();
		
		Game game = Server.getPlayerCurrentGame(playerID);
		response = getRoundResults(game);
	
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
	}
	
	private GetRoundResultsResponse getRoundResults(Game game) {
		
		if(game.getRounds().size() == 0) {
			return null;
		}
		
		Round round = game.getRounds().get(game.getRounds().size()-1);
		if(round.getPlayer1Choice()== -1 || round.getPlayer2Choice()==-1) {
			if(game.getRounds().size()<2) {
				return null;
			}
			else {
				round = game.getRounds().get(game.getRounds().size()-2);
				return setResponse(game, round);
			}
		}
		else {
			return setResponse(game, round);
		}
	}
	
	private GetRoundResultsResponse setResponse(Game game, Round round) {
		GetRoundResultsResponse response = new GetRoundResultsResponse();
		
		response.setP1Payoff(round.getPlayer1Payoff());
		response.setP1Choice(round.getPlayer1Choice());
		response.setP2Payoff(round.getPlayer2Payoff());
		response.setP2Choice(round.getPlayer2Choice());
		response.setPlayer1(game.getPlayer1ID());
		response.setPlayer2(game.getPlayer2ID());
		response.setRoundNum(game.getRoundNum());
		
		return response;
	}
	
	private GeneralRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		GeneralRequest request = new GeneralRequest();
		
		request = gson.fromJson(jsonStr, GeneralRequest.class);
		
		return request;
	}
	
	private String convertToJson(GetRoundResultsResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}