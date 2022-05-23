package responses;

public class GetPlayerShouldPlayResponse {
	private boolean playerShouldPlay = false;
	private String errorMsg;
	private String partner;
	
	public boolean isPlayerShouldPlay() {
		return playerShouldPlay;
	}
	public void setPlayerShouldPlay(boolean playerShouldPlay) {
		this.playerShouldPlay = playerShouldPlay;
	}
	
	public String getErrorMsg() {
		return errorMsg;
	}
	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}
	
	public void setPartner(String partner) {
		this.partner = partner;
	}
}
