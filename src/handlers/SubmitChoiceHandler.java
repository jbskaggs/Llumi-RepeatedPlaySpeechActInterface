package handlers;

import static java.net.HttpURLConnection.*;

import java.awt.geom.RoundRectangle2D;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.*;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import handleData.WriteGameData;
import models.Game;
import models.Round;
import requests.GeneralRequest;
import requests.SubmitChoiceRequest;
import responses.GeneralResponse;
import responses.GetPlayerShouldPlayResponse;
import server.Server;

public class SubmitChoiceHandler implements HttpHandler {

	@Override
	public void handle(HttpExchange exchange) throws IOException {
		Scanner in = new Scanner(exchange.getRequestBody());
		String jsonStr = "\n";
		
		while(in.hasNextLine()) {
			String nextLine = in.nextLine();
			jsonStr += nextLine + "\n";
		}

		in.close();
		
		SubmitChoiceRequest request = convertFromJson(jsonStr);
		GeneralResponse response = new GeneralResponse();
		
		if(request.getChoice() != 0 && request.getChoice()!=1) {
			response.setErrorMsg("choice must be 0 or 1");
		}
		else {
			String playerID = request.getPlayerID();
			
			Game game = Server.getPlayerCurrentGame(playerID);
			
			if(game.isGameIsDone()) {
				response.setErrorMsg("game is done");
			}
			else {
				if(game.getPlayer1ID().equals(playerID)) {
					response.setMsg(submitChoice(game, 1, request.getChoice(), playerID));
				}
				else if(game.getPlayer2ID().equals(playerID)) {
					response.setMsg(submitChoice(game, 2, request.getChoice(), playerID));
				}
				
				if(response.getMsg() == null) {
					response.setErrorMsg("player should not play");
				}
			}
		
		}
		
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
	}
	
	private String submitChoice(Game game, int playerNum, int choice, String playerID) {
		Round round = getRoundToPlayIn(game,playerNum);
		
		if(round == null) {
			return "player should not play";
		}
		
		if(playerNum==1) {
			round.setPlayer1Choice(choice);
		}
		else {
			round.setPlayer2Choice(choice);
		}
		
		if(roundIsDone(round)) {
			handleEndRound(game,round);
		}
		
		/*
		String partner = "";
		if (game.getPlayer1ID().equals(playerID)) {
			partner = game.getPlayer2ID();
		}
		if (game.getPlayer2ID().equals(playerID)) {
			partner = game.getPlayer1ID();
		}
				
		Server.addSubmittedPlayer(partner);
		*/	
		return "successfully submitted";
	}
	
	private Round getRoundToPlayIn(Game game, int playerNum) {
		if (game.getRounds().size()==0){
			Round round = new Round();
			game.addRound(round);
			return round;
		}
		
		Round round = game.getRounds().get(game.getRounds().size()-1);
		
		if(playerNum==1) {
			if(round.getPlayer1Choice() == -1) {
				return round;
			}
			else if(round.getPlayer1Choice() != -1 && round.getPlayer2Choice() == -1) {
				return null;
			}
			else{ //they've both played
				return makeNewRound(game);
			}
		}
		else {
			if(round.getPlayer2Choice() == -1) {
				return round;
			}
			else if(round.getPlayer2Choice() != -1 && round.getPlayer1Choice() == -1) {
				return null;
			}
			else{ //they've both played	
				return makeNewRound(game);
			}
		}
	}
	
	private boolean roundIsDone(Round round) {
		if(round.getPlayer1Choice() != -1 && round.getPlayer2Choice() != -1) {
			return true;
		}
		else {
			return false;
		}
	}
	
	private Round makeNewRound(Game game) {
		Round newRound = new Round();
		newRound.setRoundNum(game.getRoundNum());
		game.addRound(newRound);
		Server.addSubmittedPlayer(game.getPlayer1ID());
		Server.addSubmittedPlayer(game.getPlayer2ID());
		
		return newRound;
	}
	
	private void handleEndRound(Game game, Round round) {
	
		ArrayList<ArrayList<ArrayList<Integer>>> matrix = new ArrayList<ArrayList<ArrayList<Integer>>>();
		matrix = Server.getCollectionOfGamesMatrices().get(game.getGameID()).get(game.getRoundNum());
		
		ArrayList<Integer> payoff = matrix.get(round.getPlayer1Choice()).get(round.getPlayer2Choice());
		round.setPlayer1Payoff(payoff.get(0));
		round.setPlayer2Payoff(payoff.get(1));

		game.setRoundNum(game.getRoundNum()+1);
		if(game.getRounds().size()<Server.getCollectionOfGamesMatrices().get(game.getGameID()).size()) {
			makeNewRound(game);
		}
		else {
			Server.addSubmittedPlayer(game.getPlayer1ID());
			Server.addSubmittedPlayer(game.getPlayer2ID());
			handleEndGame(game);
		}
	}
	
	private void handleEndGame(Game game) {
		WriteGameData.write(game);
		WriteGameData.writeMsg(game);
		WriteGameData.writeActionMsg(game);
		game.setGameIsDone(true);
		String p1 = game.getPlayer1ID();
		String p2 = game.getPlayer2ID();
		Server.getBusyPlayers().remove(p1);
		Server.getBusyPlayers().remove(p2);
//		Server.addReadyPlayer(p1);
//		Server.addReadyPlayer(p2);
//		Server.setUpGames();
		//Check to see if next game session can start
	}
	
	
	private SubmitChoiceRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		SubmitChoiceRequest request = new SubmitChoiceRequest();
		
		try {
			request = gson.fromJson(jsonStr, SubmitChoiceRequest.class);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println();
		}
		
		return request;
	}
	
	private String convertToJson(GeneralResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}