package responses;

import java.util.ArrayList;

public class GetMatrixResponse {
	private ArrayList<ArrayList<ArrayList<Integer>>> matrix = new ArrayList<ArrayList<ArrayList<Integer>>>();
	private int playerNum = -1;
	private int roundNum = -1;
	private String errorMsg;
	
	public ArrayList<ArrayList<ArrayList<Integer>>> getMatrix() {
		return matrix;
	}
	public void setMatrix(ArrayList<ArrayList<ArrayList<Integer>>> matrix) {
		this.matrix = matrix;
	}
	

	public int getPlayerNum() {
		return playerNum;
	}
	public void setPlayerNum(int playerNum) {
		this.playerNum = playerNum;
	}

	public int getRoundNum() {
		return roundNum;
	}
	public void setRoundNum(int roundNum) {
		this.roundNum = roundNum;
	}
	
	public String getErrorMsg() {
		return errorMsg;
	}
	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}

}
