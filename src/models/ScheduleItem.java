package models;

import java.util.ArrayList;

/**
 * A schedule item consists of a user and the order of games it will play in a session
 *
 */

public class ScheduleItem {
	private String userID;
	private ArrayList<String> scheduledGames = new ArrayList<String>();
	
	public String getUserID() {
		return userID;
	}
	
	public void setUserID(String userID) {
		this.userID = userID;
	}
	
	public ArrayList<String> getSchedule() {
		return scheduledGames;
	}
	
	public void setSchedule(ArrayList<String> scheduledGames) {
		this.scheduledGames = new ArrayList<String>(scheduledGames);
	}
	
	public void addToScheduledGames(String scheduledGame) {
		scheduledGames.add(scheduledGame);
	}
	
}
