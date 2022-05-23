package responses;

public class GetIsChatEnabledResponse {
	private boolean isEnabled;
	private String errorMsg;
	
	public boolean getIsEnabled() {
		return isEnabled;
	}
	public void setIsEnabled(boolean isEnabled) {
		this.isEnabled = isEnabled;
	}
	
	public String getErrorMsg() {
		return errorMsg;
	}
	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}
}
