package handlers;

import static java.net.HttpURLConnection.*;

import static java.net.HttpURLConnection.*;

import java.io.File;
import java.io.IOException;
import java.nio.file.*;

import java.util.List;
import java.util.Map;
import java.util.Scanner;

import com.sun.net.httpserver.Headers;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
public class RootHandler implements HttpHandler{

	@Override
	public void handle(HttpExchange exchange) throws IOException {
		/*
		System.out.println("Server: root handler");
		System.out.println();

		System.out.println("request:");
		System.out.println("method: " + exchange.getRequestMethod());
		System.out.println("uri: " + exchange.getRequestURI());
		System.out.println();

		printHeaders(exchange.getRequestHeaders());

		System.out.println("request body: ");
		Scanner in = new Scanner(exchange.getRequestBody());
		while(in.hasNextLine()) {
			System.out.println(in.nextLine());
		}
		in.close();
		System.out.println("end request body\n");
		*/
		try {
			String filePathStr = "";
			String totalURL = exchange.getRequestURI().toString();

			if(totalURL.equals("/")) {
				filePathStr = "./web/login.html";
			}
			else {
				filePathStr = "./web/" + exchange.getRequestURI().toString();
				//System.out.println("FPA: " + filePathStr);
			}

			//System.out.println(totalURL + " " + totalURL.getClass());

			File temp = new File(filePathStr);

			if(temp.exists()) {
				exchange.sendResponseHeaders(HTTP_OK, 0);

				Path filePath = FileSystems.getDefault().getPath(filePathStr);
				Files.copy(filePath, exchange.getResponseBody());

				exchange.getResponseBody().close();

			}
			else {
				exchange.sendResponseHeaders(HTTP_BAD_REQUEST, 0);
				exchange.getResponseBody().close();
			}

		} catch (Exception e) {
			System.out.println("file handle failure");
			e.printStackTrace();
		}

		//System.out.println("SERVER: root handler done");
	}

	void printHeaders(Headers headers) {

		System.out.println("request headers:");

		for (String name : headers.keySet())
		    System.out.println(name + " = " + headers.get(name));

		System.out.println();
	}

}
