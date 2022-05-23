package handlers;

import static java.net.HttpURLConnection.*;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import models.Game;
import models.Round;
import requests.GeneralRequest;
import responses.GetIsChatEnabledResponse;
import responses.GetMatrixResponse;
import responses.GetPlayerShouldPlayResponse;
import server.Server;

public class GetMatrixHandler implements HttpHandler {

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
		GetMatrixResponse response = new GetMatrixResponse();
		
		String playerID = request.getPlayerID();
				
		Game game = Server.getPlayerCurrentGame(playerID);
		if(game.getPlayer1ID().equals(playerID)) {
			response.setMatrix(getMatrix(game,1));
			response.setPlayerNum(1);
		}
		else if(game.getPlayer2ID().equals(playerID)) {
			response.setMatrix(getMatrix(game,2));
			response.setPlayerNum(2);
		}
		
		response.setRoundNum(game.getRoundNum());
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
	}
	
	private ArrayList<ArrayList<ArrayList<Integer>>> getMatrix(Game game, int playerNum){
		int gameID = game.getGameID();
		int roundNum = game.getRoundNum();
		
		ArrayList<ArrayList<ArrayList<Integer>>> matrix = Server.getCollectionOfGamesMatrices().get(gameID).get(roundNum);
		return matrix;
	}
	
	private GeneralRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		GeneralRequest request = new GeneralRequest();
		
		request = gson.fromJson(jsonStr, GeneralRequest.class);
		
		return request;
	}
	
	private String convertToJson(GetMatrixResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}