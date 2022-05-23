package responses;

public class GetRoundResultsResponse {
	private int roundNum = -1;
	private String player1;
	private String player2;
	private int p1Payoff;
	private int p1Choice;
	private int p2Payoff;
	private int p2Choice;
	
	public int getRoundNum() {
		return roundNum;
	}
	public void setRoundNum(int roundNum) {
		this.roundNum = roundNum;
	}
	
	public String getPlayer1() {
		return player1;
	}
	public void setPlayer1(String player1) {
		this.player1 = player1;
	}
	
	public String getPlayer2() {
		return player2;
	}
	public void setPlayer2(String player2) {
		this.player2 = player2;
	}
	
	public int getP1Payoff() {
		return p1Payoff;
	}
	public void setP1Payoff(int p1Payoff) {
		this.p1Payoff = p1Payoff;
	}
	
	public int getP1Choice() {
		return p1Choice;
	}
	public void setP1Choice(int p1Choice) {
		this.p1Choice = p1Choice;
	}
	
	public int getP2Payoff() {
		return p2Payoff;
	}
	public void setP2Payoff(int p2Payoff) {
		this.p2Payoff = p2Payoff;
	}
	
	public int getP2Choice() {
		return p2Choice;
	}
	public void setP2Choice(int p2Choice) {
		this.p2Choice = p2Choice;
	}
	
}
