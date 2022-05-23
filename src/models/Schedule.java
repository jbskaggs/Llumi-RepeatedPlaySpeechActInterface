package models;

import java.util.ArrayList;

public class Schedule {
	private boolean enableCheapTalk = false;
	
	private ArrayList<ScheduleItem> scheduleItems = new ArrayList<ScheduleItem>();
	
	public boolean isEnableCheapTalk() {
		return enableCheapTalk;
	}
	public void setEnableCheapTalk(boolean enableCheapTalk) {
		this.enableCheapTalk = enableCheapTalk;
	}
	
	public ArrayList<ScheduleItem> getScheduleItems() {
		return scheduleItems;
	}
	public void setScheduleItems(ArrayList<ScheduleItem> scheduleItems) {
		this.scheduleItems = scheduleItems;
	}
	public void addScheduleItem(ScheduleItem item) {
		this.scheduleItems.add(item);
	}
	
	
}
