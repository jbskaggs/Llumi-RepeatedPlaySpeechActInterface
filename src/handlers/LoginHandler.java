package handlers;

import static java.net.HttpURLConnection.*;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;
import java.util.Scanner;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import requests.LoginRequest;
import responses.LoginResponse;
import server.Server;

public class LoginHandler implements HttpHandler {

	@Override
	public void handle(HttpExchange exchange) throws IOException {

		Scanner in = new Scanner(exchange.getRequestBody());
		String jsonStr = "\n";

		while(in.hasNextLine()) {
			String nextLine = in.nextLine();
			jsonStr += nextLine + "\n";
		}

		in.close();

		LoginRequest loginRequest = this.convertFromJson(jsonStr);
		LoginResponse loginResponse = new LoginResponse();

		if(loginRequest.getErrorMsg() == null) {
			boolean added = false;

			if(Server.getReadyPlayers().contains(loginRequest.getPlayerID())){
				loginResponse.setErrorMsg("user already logged in");
			}
			else if(Server.getBusyPlayers().contains(loginRequest.getPlayerID())) {
				loginResponse.setErrorMsg("user already logged in");
			}
			else {
				for(int i=0; i<Server.getSchedule().getScheduleItems().size(); i++) {
					String playerID = loginRequest.getPlayerID();
					String key = Server.getSchedule().getScheduleItems().get(i).getUserID();
					if(playerID.equals(key)) {
						if(!Server.getBusyPlayers().contains(playerID)) {
							Server.addReadyPlayer(playerID);
							loginResponse.setMsg("user already logged in");
						}
						Server.setUpGames();
						added = true;
						loginResponse.setMsg("successfully logged in");
					}
				}

				if(!added) {
					loginResponse.setErrorMsg("invalid input");
				}
			}

			exchange.sendResponseHeaders(HTTP_OK, 0);
		}
		else {
			loginResponse = new LoginResponse();
			loginResponse.setErrorMsg(loginRequest.getErrorMsg());
			exchange.sendResponseHeaders(HTTP_OK, 0);
		}

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(loginResponse));
		out.close();

	}

	private LoginRequest convertFromJson(String jsonStr) {
		Gson gson = new Gson();
		LoginRequest loginRequest = new LoginRequest();

		try {
			loginRequest = gson.fromJson(jsonStr, LoginRequest.class);
		} catch (Exception e) {
			loginRequest.setErrorMsg("Bad param(s)");
		}

		return loginRequest;
	}

	private String convertToJson(LoginResponse loginResponse) {
		Gson gson = new Gson();
		String jsonStr = "";

		jsonStr = gson.toJson(loginResponse);

		return jsonStr;
	}

}
