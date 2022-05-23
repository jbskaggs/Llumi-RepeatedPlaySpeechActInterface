package handlers;

import static java.net.HttpURLConnection.*;

import java.io.IOException;
import java.io.PrintWriter;

import com.google.gson.Gson;
import com.sun.net.httpserver.*;

import responses.GetIsChatEnabledResponse;
import server.Server;

public class GetIsChatEnabledHandler implements HttpHandler {

	@Override
	public void handle(HttpExchange exchange) throws IOException {
		GetIsChatEnabledResponse response = new GetIsChatEnabledResponse();
		
		response.setIsEnabled(Server.getSchedule().isEnableCheapTalk());
		
		exchange.sendResponseHeaders(HTTP_OK, 0);

		PrintWriter out = new PrintWriter(exchange.getResponseBody());
		out.print(this.convertToJson(response));
		out.close();
	}
	
	private String convertToJson(GetIsChatEnabledResponse response) {
		Gson gson = new Gson();
		String jsonStr = "";
		
		jsonStr = gson.toJson(response);
		
		return jsonStr;
	}
	
}