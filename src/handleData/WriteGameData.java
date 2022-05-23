package handleData;

import java.io.*;
import java.util.ArrayList;
import java.util.Map;

import models.ChatMsg;
import models.Game;
import models.Round;
import server.Server;

public class WriteGameData {
	public static void write(Game game) {
		String filePath = "./data/outfiles/game" + Integer.toString(game.getGameID()) + "_" + game.getPlayer1ID() + "_" + game.getPlayer2ID() + ".txt";
		File writeFile = new File(filePath);
		PrintWriter writer = null;
		
		try {
			writer = new PrintWriter(writeFile);
		} catch (FileNotFoundException e) {
			System.out.println("Program failed to write data to file");
			System.exit(1);
		}
		
		int player1Total = 0;
		int player2Total = 0;
		
		//Print to writer here
		for(Round round:game.getRounds()) {
			writer.println(round.getRoundStartTime() + " " + round.getPlayer1SubmitTime() + " " + round.getPlayer1Choice() + " " + round.getPlayer1Payoff() + " " 
					+ round.getPlayer2SubmitTime() + " " + round.getPlayer2Choice() + " " + round.getPlayer2Payoff());
			
			player1Total += round.getPlayer1Payoff();
			player2Total += round.getPlayer2Payoff();
		}
		writer.println(player1Total + " " + player2Total);
		writer.println(player1Total * 0.01 + " " + player2Total * 0.01);
		
		writer.close();
		
		System.out.println("Game removed");
	}
	
	public static void writeMsg(Game game) {
		ArrayList<ChatMsg> msgs = game.getMessages();
		ChatMsg msg1, msg2;
		String player1ID = game.getPlayer1ID();
		String player2ID = game.getPlayer2ID();
		
	
		//print_to_file
		String filePath = "./data/outfiles/game" + Integer.toString(game.getGameID()) + "_" + player1ID + "_" + player2ID + "_msg.txt";
		File writeFile = new File(filePath);
		PrintWriter writer = null;
		
		try {
			writer = new PrintWriter(writeFile);
		} catch (FileNotFoundException e) {
			System.out.println("Program failed to write data to file");
			System.exit(1);
		}
		
		int i = 0;
		int size = msgs.size();
		
		while (i < size) {
			if (msgs.get(i).getPlayerID().equals(player1ID)) {
				msg1 = msgs.get(i);
				msg2 = msgs.get(i+1);
			} 
			else {
				msg1 = msgs.get(i+1);
				msg2 = msgs.get(i);
			}
			
			//Print to writer here		
			writer.println(msg1.getSentTime() + " " + msg1.getMessage() + " " + msg2.getSentTime() + " " + msg2.getMessage());
			
			i += 2;
		}
	
		writer.close();
		
		System.out.println("Time added");
		
		
	}
	
	public static void writeActionMsg(Game game) {
		ArrayList<ChatMsg> msgs = game.getMessages();
		ChatMsg msg1, msg2;
		String player1ID = game.getPlayer1ID();
		String player2ID = game.getPlayer2ID();
		
	
		//print_to_file
		String filePath = "./data/outfiles/game" + Integer.toString(game.getGameID()) + "_" + player1ID + "_" + player2ID + "_actmsg.csv";
		File writeFile = new File(filePath);
		PrintWriter writer = null;
		
		try {
			writer = new PrintWriter(writeFile);
		} catch (FileNotFoundException e) {
			System.out.println("Program failed to write data to file");
			System.exit(1);
		}
		
		int i = 0; //msg count
		int j = 0; //round count
		int size = msgs.size();
		ArrayList<Round> rounds = game.getRounds();
		
		writer.println("Round, P1 Action, P2 Action, P1 Msg, P2 Msg");
		
		while (i < size) {
			if (msgs.get(i).getPlayerID().equals(player1ID)) {
				msg1 = msgs.get(i);
				msg2 = msgs.get(i+1);
			} 
			else {
				msg1 = msgs.get(i+1);
				msg2 = msgs.get(i);
			}
			Round round = rounds.get(j);
			//Print to writer here		
			writer.println(j + ", " + round.getPlayer1Choice() + ", " + round.getPlayer2Choice() + ", " + msg1.getMessage() + ", " + msg2.getMessage());
			
			i += 2;
			j += 1;
		}
	
		writer.close();
		
		System.out.println("Print out action msg pair");
	}
}
