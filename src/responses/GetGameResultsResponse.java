package responses;

public class GetGameResultsResponse {
	private int gameID;
	private int numRoundsCompleted;
	private int numRoundsTotal;
	private int summedPayoff;
	private String errorMsg;
	
	public int getGameID() {
		return gameID;
	}
	public void setGameID(int i) {
		this.gameID = i;
	}
	
	public int getNumRoundsCompleted() {
		return numRoundsCompleted;
	}
	public void setNumRoundsCompleted(int numRoundsCompleted) {
		this.numRoundsCompleted = numRoundsCompleted;
	}
	
	public int getNumRoundsTotal() {
		return numRoundsTotal;
	}
	public void setNumRoundsTotal(int numRoundsTotal) {
		this.numRoundsTotal = numRoundsTotal;
	}
	
	public int getSummedPayoff() {
		return summedPayoff;
	}
	public void setSummedPayoff(int summedPayoff) {
		this.summedPayoff = summedPayoff;
	}
	
	public String getErrorMsg() {
		return errorMsg;
	}
	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}
}
