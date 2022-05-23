package models;

import java.time.*;

public class Round {
	private int roundNum;
	private int player1Choice = -1; // Either 0 or 1
	private int player2Choice = -1; // Either 0 or 1
	private int player1Payoff = -1; // From game json
	private int player2Payoff = -1; // From game json
	private LocalTime roundStart, player1SubmitTime, player2SubmitTime;
	
	public Round() {
		roundStart = LocalTime.now();
	}
	
	public int getRoundNum() {
		return roundNum;
	}
	public void setRoundNum(int roundNum) {
		this.roundNum = roundNum;
	}
	
	public int getPlayer1Choice() {
		return player1Choice;
	}
	public void setPlayer1Choice(int player1Choice) {
		player1SubmitTime = LocalTime.now();
		this.player1Choice = player1Choice;
	}
	
	public int getPlayer2Choice() {
		return player2Choice;
	}
	public void setPlayer2Choice(int player2Choice) {
		player2SubmitTime = LocalTime.now();
		this.player2Choice = player2Choice;
	}
	
	public int getPlayer1Payoff() {
		return player1Payoff;
	}
	public void setPlayer1Payoff(int player1Payoff) {
		this.player1Payoff = player1Payoff;
	}
	
	public int getPlayer2Payoff() {
		return player2Payoff;
	}
	public void setPlayer2Payoff(int player2Payoff) {
		this.player2Payoff = player2Payoff;
	}
	
	public LocalTime getRoundStartTime() {
		return roundStart;
	}
	public LocalTime getPlayer1SubmitTime() {
		return player1SubmitTime;
	}
	public LocalTime getPlayer2SubmitTime() {
		return player2SubmitTime;
	}
}
