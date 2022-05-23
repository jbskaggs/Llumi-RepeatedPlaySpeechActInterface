package handlers;

import static java.net.HttpURLConnection.*;

import java.io.IOException;
import java.io.PrintWriter;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import responses.GetServerScheduleResponse;
import server.Server;

public class GetServerScheduleHandler implements HttpHandler {

	@Override
	public void handle(HttpExchange exchange) throws IOException {
		GetServerScheduleResponse response = new GetServerScheduleResponse();
			
		response.setSchedule(Server.getSchedule());
		
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
	}
	
	private String convertToJson(GetServerScheduleResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}