package models;

import java.time.*;

public class ChatMsg {
	private String playerID;
	//private int gameID = -1;
	//private int roundNum = -1;
	private LocalTime sentTime;
	private String message;
	
	
	public ChatMsg() {
		sentTime = LocalTime.now();
	}
	
	public String getPlayerID() {
		return playerID;
	}
	public void setPlayerID(String playerID) {
		this.playerID = playerID;
	}
	/*
	public int getGameID() {
		return gameID;
	}
	public void setGameID(int gameID) {
		this.gameID = gameID;
	}
	
	public int getRoundNum() {
		return roundNum;
	}
	public void setRoundNum(int roundNum) {
		this.roundNum = roundNum;
	}
	*/
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
	
	public LocalTime getSentTime() {
		return sentTime;
	}
	
}
