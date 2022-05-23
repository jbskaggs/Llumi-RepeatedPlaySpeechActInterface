package server;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.*;
import java.util.*;
import java.util.Random;


import com.sun.net.httpserver.*;

import handleData.ParseGameJson;
import handleData.ParseGamesTxt;
import handleData.ParseScheduleJson;
import handlers.*;
import models.*;

public class Server {
	private static Schedule schedule = new Schedule();
	private static ArrayList<String> readyPlayers = new ArrayList<String>(); //Players waiting to be paired into game
	private static ArrayList<String> busyPlayers = new ArrayList<String>();  //Players in a game, either waiting or actively playing
	private static ArrayList<String> donePlayers = new ArrayList<String>();  //Players that are done with all of their games
	private static ArrayList<String> messagedPlayers = new ArrayList<String>();
	private static ArrayList<String> submittedPlayers = new ArrayList<String>();
	private static ArrayList<File> gameJsonFiles = new ArrayList<File>();
	private static ArrayList<Game> serverGames = new ArrayList<Game>();
	private static ArrayList<ArrayList<ArrayList<ArrayList<ArrayList<Integer>>>>> collectionOfGamesMatrices = new ArrayList<ArrayList<ArrayList<ArrayList<ArrayList<Integer>>>>>();
	private static ArrayList<String []> argsAI = new ArrayList<String []>();
	private static Map<String, String> outputs = new HashMap<>();
	private static boolean sessionIsDone = false;
	private static int currentGame = 0;	/*Game index starts at zero*/

	public static void main(String[] args) throws Exception {
		int port = 8888;
		System.out.println("Server listening on port: " + port);
		System.out.println();

		if(args.length > 2) {
			shutDownProgram("Usage: ./server");
		}

		String game_src = "./data/games.txt";
		String schedule_src = "./data/schedule.json";
		loadData(game_src, schedule_src);

		HttpServer server = HttpServer.create(new InetSocketAddress(port),0);

		try {
			server.createContext("/",new RootHandler());
			server.createContext("/login", new LoginHandler());
			server.createContext("/checkStatus", new CheckStatusHandler());
			server.createContext("/getIsChatEnabled", new GetIsChatEnabledHandler());
			server.createContext("/getServerSchedule", new GetServerScheduleHandler());
			server.createContext("/getPlayerShouldPlay", new GetPlayerShouldPlayHandler());
			server.createContext("/getMatrix", new GetMatrixHandler());
			server.createContext("/submitChoice", new SubmitChoiceHandler());
			server.createContext("/getRoundResults", new GetRoundResultsHandler());
			server.createContext("/getGameResults", new GetGameResultsHandler());
			server.createContext("/checkMessageStatus", new CheckMessageStatusHandler());
			server.createContext("/checkSubmittedStatus", new CheckSubmittedStatusHandler());
			server.createContext("/sendChatMsg", new SendChatMsgHandler());
			server.createContext("/retrieveMsgs", new RetrieveMsgsHandler());
		} catch (Exception e) {
			e.printStackTrace();
			shutDownProgram("");
		}

		server.start();
	}

	public static void setUpGames() {

		System.out.println("Setting Up Game...");

		if(Server.currentGame > Server.schedule.getScheduleItems().get(0).getSchedule().size()-1) {
			for(String player:Server.readyPlayers) {
				Server.donePlayers.add(player);
			}

			Server.readyPlayers.clear();

			//If all players done
			if(Server.donePlayers.size()>=Server.schedule.getScheduleItems().size()) {
				Server.setSessionIsDone(true);
				System.out.println("The user study is done");
			}

			return;
		}

		// create instance of Random class
        Random rand = new Random();

		ArrayList<ScheduleItem> items = Server.getSchedule().getScheduleItems();

		if(readyPlayers.size() == Server.schedule.getScheduleItems().size()) {
			System.out.println("Inside the first if");

			while(readyPlayers.size()>0){
				String me = readyPlayers.get(0);
				String partner = "";

				for(int j=0; j<items.size(); j++) {
					if(me.equals(items.get(j).getUserID())) {
						partner = items.get(j).getSchedule().get(currentGame);
						break;
					}
				}

				if(!partner.contains("player")) {
					System.out.println("starting algorithm: " + partner + " ...");

					System.out.println("pairing: " + readyPlayers.get(0) + " and " + partner);

					String partnerID = partner + Character.toString(me.charAt(me.length()-1));

					String isEnableCheapTalk = Boolean.toString(Server.schedule.isEnableCheapTalk());
					
					//if (partner.equals("afp")) {
					//Automatically starting AI
					String[] args = new String[] {"./src/Algs_new/Solver", partnerID, partner, "1", isEnableCheapTalk};
					argsAI.add(args);
					//}

					//pair AI and serverGames
					readyPlayers.remove(me);
					busyPlayers.add(me);
					busyPlayers.add(partnerID);
					addGame(me, partnerID);		
				}
				else {
					System.out.println("pairing: " + readyPlayers.get(0) + " and " + partner);
					readyPlayers.remove(me);
					readyPlayers.remove(partner);
					busyPlayers.add(me);
					busyPlayers.add(partner);
					
					Integer player_id = Integer.valueOf(me.substring(me.length()-1));
					if (player_id % 2 == 1) {
						addGame(me, partner);
					}
					else {
						addGame(partner, me);
					}
				}
			}
			startGames();
		}

	}

	public static Game getPlayerCurrentGame(String playerID) {
		for(int i=Server.getServerGames().size()-1; i>= 0 ;i--) {
			Game game = Server.getServerGames().get(i);
			if(game.getPlayer1ID().equals(playerID)) {
				return game;
			}
			else if (game.getPlayer2ID().equals(playerID)) {
				return game;
			}
		}

		return null;
	}

	private static void addGame(String player1ID, String player2ID) {
		Game game = new Game();
		game.setPlayer1ID(player1ID);
		game.setPlayer2ID(player2ID);
		game.setGameID(Server.currentGame);

		Server.addServerGame(game);
	}

	private static void startGames() {
		//start AIs when all players are ready
		for (String [] args:Server.argsAI) {
			try {
				String time = java.time.LocalDateTime.now().toString();
				File output = new File("./data/outfiles/game" + Integer.toString(Server.currentGame) + "_" + args[1] + "_" + time + ".txt");
				
				ProcessBuilder pb = new ProcessBuilder(args);
				pb.redirectOutput(output);
				pb.start();
			} catch (IOException e) {
				System.out.println("Unable to start AI");
				e.printStackTrace();
				shutDownProgram("");
			}
		}

		Server.argsAI.clear();
		
		Server.currentGame += 1; //Test to make sure nothing besides the creation of games is reliant on this
	}

	private static void loadData(String gamesFP, String scheduleFP) {

		File gamesTxt = new File(gamesFP);
		if(gamesTxt.exists()) {
			try {
				ParseGamesTxt.parse(gamesTxt);

				for(int i=0; i<gameJsonFiles.size(); i++) {
					ParseGameJson.parse(gameJsonFiles.get(i),i);
				}

			} catch (Exception e) {
				e.printStackTrace();
				shutDownProgram("");
			}
		}
		else {
			shutDownProgram("Loading games.txt file failed");
		}

		File scheduleJson = new File(scheduleFP);
		if(scheduleJson.exists()) {
			try {
				ParseScheduleJson.parse(scheduleJson);
			} catch (Exception e) {
				e.printStackTrace();
				shutDownProgram("");
			}
		}
		else {
			shutDownProgram("Loading schedule.json failed");
		}
	}

	private static void shutDownProgram(String errorMsg) {
		System.out.println(errorMsg);
		System.out.println("Closing program ...");
		System.exit(0);
	}


	/*
	 * HANDLERS
	 */

	public static Schedule getSchedule() {
		return schedule;
	}
	public static void setSchedule(Schedule schedule) {
		Server.schedule = schedule;
	}
	public static void addScheduleItemToSchedule(ScheduleItem item) {
		Server.schedule.addScheduleItem(item);
	}

	public static ArrayList<String> getReadyPlayers() {
		return readyPlayers;
	}
	public static void setReadyPlayers(ArrayList<String> readyPlayers) {
		Server.readyPlayers = readyPlayers;
	}
	public static void addReadyPlayer(String readyPlayer) {
		Server.readyPlayers.add(readyPlayer);
	}

	public static ArrayList<String> getDonePlayers() {
		return donePlayers;
	}

	public static ArrayList<String> getBusyPlayers() {
		return busyPlayers;
	}
	public static void setBusyPlayers(ArrayList<String> busyPlayers) {
		Server.busyPlayers = busyPlayers;
	}
	public static void addBusyPlayer(String busyPlayer) {
		Server.busyPlayers.add(busyPlayer);
	}

	public static void removeMessagedPlayer(String messagedPlayer) {
		Server.messagedPlayers.remove(messagedPlayer);
	}
	public static void addMeassgedPlayer(String messagedPlayer) {
		Server.messagedPlayers.add(messagedPlayer);
	}
	public static ArrayList<String> getMessagedPlayers() {
		return messagedPlayers;
	}

	public static void removeSubmittedPlayer(String messagedPlayer) {
		Server.submittedPlayers.remove(messagedPlayer);
	}
	public static void addSubmittedPlayer(String messagedPlayer) {
		Server.submittedPlayers.add(messagedPlayer);
	}
	public static ArrayList<String> getSubmittedPlayers() {
		return submittedPlayers;
	}

	public static ArrayList<File> getGameJsonFiles() {
		return gameJsonFiles;
	}
	public static void setGameJsonFiles(ArrayList<File> gameJsonFiles) {
		Server.gameJsonFiles = gameJsonFiles;
	}
	public static void addGameJsonFile(File file) {
		Server.gameJsonFiles.add(file);
	}

	public static ArrayList<Game> getServerGames() {
		return serverGames;
	}
	public static void setServerGames(ArrayList<Game> serverGames) {
		Server.serverGames = serverGames;
	}
	public static void addServerGame(Game game) {
		Server.serverGames.add(game);
	}

	public static ArrayList<ArrayList<ArrayList<ArrayList<ArrayList<Integer>>>>> getCollectionOfGamesMatrices() {
		return collectionOfGamesMatrices;
	}
	public static void setGameMatrices(ArrayList<ArrayList<ArrayList<ArrayList<ArrayList<Integer>>>>> collectionOfGamesMatrices) {
		Server.collectionOfGamesMatrices = collectionOfGamesMatrices;
	}
	public static void addGamesMatrices(ArrayList<ArrayList<ArrayList<ArrayList<Integer>>>> collectionOfMatrices) {
		Server.collectionOfGamesMatrices.add(collectionOfMatrices);
	}

	public static boolean isSessionIsDone() {
		return sessionIsDone;
	}

	public static void setSessionIsDone(boolean sessionIsDone) {
		Server.sessionIsDone = sessionIsDone;
	}

	public static int getCurrentGame() {
		return currentGame;
	}
	public static void setCurrentGame(int currentGame) {
		Server.currentGame = currentGame;
	}
	
	public static Map<String,String> getAIOutput() {
		return Server.outputs;
	}
	public static void clearAIOutput() {
		Server.outputs.clear();
	}

}
