package handleData;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

import models.Game;
import models.Round;
import server.Server;

public class ParseGamesTxt {
	public static void parse(File file) throws Exception {
		Scanner scanner = new Scanner(file);
		System.out.println("1");
		
		String filePath = "./src/server/currentGameString.txt";
		File writeFile = new File(filePath);
		PrintWriter writer = null;
		
		try {
			writer = new PrintWriter(writeFile);
		} catch (FileNotFoundException e) {
			System.out.println("Program failed to write data to file");
			System.exit(1);
		}
		
		while(scanner.hasNextLine()) {
			String tempStr = scanner.nextLine();
			writer.println(tempStr);
			
			tempStr = "./data/interactions/" + tempStr + ".json";
			
			File tempFile = new File(tempStr);
			if(tempFile.exists()) {
				Server.addGameJsonFile(tempFile);
			}
			else {
				System.out.println(tempStr + " doesn't exist");
				throw new Exception();
			}
		}
		
		scanner.close();
		writer.close();
	}
	
}
