package responses;

import java.util.ArrayList;

import models.ChatMsg;

public class RetrieveMsgsResponse {
	private ArrayList<ChatMsg> chatMsgs;
	private String errorMsg;
	
	public ArrayList<ChatMsg> getChatMsgs() {
		return chatMsgs;
	}
	public void setChatMsgs(ArrayList<ChatMsg> chatMsgs) {
		this.chatMsgs = chatMsgs;
	}
	
	public String getErrorMsg() {
		return errorMsg;
	}
	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}
}
