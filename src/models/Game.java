package models;

import java.util.ArrayList;

public class Game {
	private String player1ID;
	private String player2ID;
	private int roundNum = 0;
	private int gameID = -1;
	private ArrayList<Round> rounds = new ArrayList<Round>();
	private ArrayList<ChatMsg> messages = new ArrayList<ChatMsg>();
	private boolean gameIsDone = false;
	
	public String getPlayer1ID() {
		return player1ID;
	}
	public void setPlayer1ID(String player1id) {
		player1ID = player1id;
	}
	
	public String getPlayer2ID() {
		return player2ID;
	}
	public void setPlayer2ID(String player2id) {
		player2ID = player2id;
	}
	
	public int getRoundNum() {
		return roundNum;
	}
	public void setRoundNum(int roundNum) {
		this.roundNum = roundNum;
	}
	
	public int getGameID() {
		return gameID;
	}
	public void setGameID(int gameID) {
		this.gameID = gameID;
	}
	
	public ArrayList<Round> getRounds() {
		return rounds;
	}
	public void setRounds(ArrayList<Round> rounds) {
		this.rounds = rounds;
	}
	public void addRound(Round round) {
		this.rounds.add(round);
	}
	
	public ArrayList<ChatMsg> getMessages() {
		return messages;
	}
	public void setMessages(ArrayList<ChatMsg> messages) {
		this.messages = messages;
	}
	public void addMessage(ChatMsg message) {
		this.messages.add(message);
	}
	
	public boolean isGameIsDone() {
		return gameIsDone;
	}
	public void setGameIsDone(boolean gameIsDone) {
		this.gameIsDone = gameIsDone;
	}
}
