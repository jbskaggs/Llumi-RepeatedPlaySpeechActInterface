package handleData;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import models.*;
import server.Server;

public class ParseScheduleJson {
	public static void parse(File file) throws FileNotFoundException {
		Scanner scanner = new Scanner(file);
		String jsonStr = "";
		
		while(scanner.hasNextLine()) {
			String line = scanner.nextLine();
			
			if(line.charAt(0) != '}') {
				parseLine(line);
			}
			
		}
		
	}
	
	private static void parseLine(String line) {
		StringBuilder tempStr = new StringBuilder();
		
		int i = 0;
		
		//If first line in json, get rid of {
		if(line.charAt(i)=='{') {
			i++;
		}
		
		//Remove first quote
		i++;
		
		//Get the key
		tempStr = new StringBuilder();
		while(Character.isLetterOrDigit(line.charAt(i))) {
			tempStr.append(line.charAt(i));
			i++;
		}
		
		//Handle the key
		if(tempStr.toString().equals("cheaptalk")) {
			setCheaptalk(line);
		}
		else {
			//Add key to ScheduleItem object
			ScheduleItem scheduleItem = new ScheduleItem();
			scheduleItem.setUserID(tempStr.toString());
			//Remove quote at end of key
			i++;
			
			//Read until next quote
			while(line.charAt(i) != '"') {
				i++;
			}
			
			//Read in the schedule
			while(i < line.length()) {
				if(Character.isLetterOrDigit(line.charAt(i))) {
					tempStr = new StringBuilder();
					while(Character.isLetterOrDigit(line.charAt(i))) {
						tempStr.append(line.charAt(i));
						i++;
					}
					scheduleItem.addToScheduledGames(tempStr.toString());
				}
				else {
					i++;
				}
			}
			
			Server.addScheduleItemToSchedule(scheduleItem);
		}
		
	}
	
	private static void setCheaptalk(String line) {
		if(line.contains("true")) {
			Server.getSchedule().setEnableCheapTalk(true);
		}
		else if(line.contains("false")) {
			Server.getSchedule().setEnableCheapTalk(false);
		}
		
	}
	
	
}
