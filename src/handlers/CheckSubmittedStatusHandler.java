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
import responses.CheckMessageStatusResponse;
import responses.GeneralResponse;
import server.Server;

public class CheckSubmittedStatusHandler implements HttpHandler {

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
		CheckMessageStatusResponse response = new CheckMessageStatusResponse();
		
		if(Server.getSubmittedPlayers().contains(request.getPlayerID())) {
			response.setStatus("Submitted");
			Server.removeSubmittedPlayer(request.getPlayerID());
		}
		else {
			response.setStatus("Not submitted");
		}
		
		exchange.sendResponseHeaders(HTTP_OK, 0);

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
	
	private String convertToJson(CheckMessageStatusResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}