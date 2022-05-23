package handleData;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

import server.Server;

public class ParseGameJson {
	public static void parse(File jsonFile, int fileNum) throws FileNotFoundException {
		Scanner scanner = new Scanner(jsonFile);
		//Get rid of {
		scanner.nextLine();
		
		//Get rid of number line, we'll just read as many as there are.
		scanner.nextLine();
		ArrayList<ArrayList<ArrayList<ArrayList<Integer>>>> gamesMatrices = new ArrayList<ArrayList<ArrayList<ArrayList<Integer>>>>(); 
		//Read lines from Json
		while(scanner.hasNextLine()) {
			ArrayList<ArrayList<ArrayList<Integer>>> outsideLevel = new ArrayList<ArrayList<ArrayList<Integer>>>(); 
			ArrayList<Integer> getNextNumRet = null;
			String jsonLine = scanner.nextLine();
			int i=0;
			
			if(jsonLine.charAt(0)=='}') {
				Server.addGamesMatrices(gamesMatrices);
				return;
			}
			
			while(jsonLine.charAt(i)!='['){
				i++;
			}
			
			//Get rid of [
			i++;
			
			for(int index1=0; index1<2; index1++) {
				ArrayList<ArrayList<Integer>> middleLevel = new ArrayList<ArrayList<Integer>>();
				for(int index2=0; index2<2; index2++) {
					ArrayList<Integer> insideLevel = new ArrayList<Integer>();
					for(int index3=0; index3<2; index3++) {
						getNextNumRet = getNextNum(jsonLine, i);
						insideLevel.add(getNextNumRet.get(0));
						i = getNextNumRet.get(1);
						
					}
					middleLevel.add(insideLevel);
				}
				outsideLevel.add(middleLevel);
			}	
			gamesMatrices.add(outsideLevel);
		}
	}
	
	/**
	 * Return:
	 * 	0 index is the number to return
	 * 	1 index is the i val
	 */
	private static ArrayList<Integer> getNextNum(String jsonLine, int i) {
		StringBuilder sb = new StringBuilder();
		ArrayList<Integer> ret = new ArrayList<Integer>();
		
		//Check out of bounds index
		if(i>=jsonLine.length()) {
			ret.add(-1);
			ret.add(i);
			return ret;
		}
		
		while(!Character.isLetterOrDigit(jsonLine.charAt(i))) {
			
			i++;
			
			//Check out of bounds index
			if(i>=jsonLine.length()) {
				ret.add(-1);
				ret.add(i);
				return ret;
			}
		}
		
		while(Character.isDigit(jsonLine.charAt(i))) {
			sb.append(jsonLine.charAt(i));
			i++;
		}
		
		ret.add(Integer.parseInt(sb.toString()));
		ret.add(i);
		return ret;
	}
}
