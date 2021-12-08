//Name: Ron Amar
//ID: 318479458

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class TextAnalyzer 
{
	
	private File file;
	private String filePath;
	private String cleanFilePath;
	private final String STOP_WORDS_PATH = "stopWords.txt";
	private List<String> stopWords;
	HashMap<String, Integer> wordsMap;
	private final int FILE_ENDING = 4;
	
	public TextAnalyzer(String textPath) throws IOException {
		if(textPath == null) throw new IOException("null path");
		//checks if the path specified has at least 1 char and a file type
		if(textPath.length() <= FILE_ENDING) throw new IOException("Path too short");
		//get the file type
		String fileType = textPath.substring(textPath.length() - FILE_ENDING);
		
		if(!fileType.equals(".txt"))
			throw new IOException("File is not a text file");
		
		file = new File(textPath);
		filePath = textPath;
		stopWords = new ArrayList<>();
	}
	
	public void cleanText() throws CleanFailException {
		//creating a path for the normalized and clean files
		String normalizedFilePath = this.filePath.substring(0, filePath.length() - FILE_ENDING);
		String cleanFilePath = normalizedFilePath;
		normalizedFilePath += "Normalize.txt";
		cleanFilePath += "Clean.txt";
		this.cleanFilePath = cleanFilePath;
		
		//declaring to be able to close them later
		FileReader fr = null;
		BufferedReader buff = null;
		FileWriter fw = null;
		
		try {
			//===============NORMALIZE===============
			//creates a new normalized file
			File normalizedFile = new File(normalizedFilePath);
			normalizedFile.createNewFile();
			
			fr = new FileReader(this.file);
			buff = new BufferedReader(fr);
			
			fw = new FileWriter(normalizedFile);
			//running on the original file line by line
			try {
				while(true) {
					boolean startOfSentence = true;
					String temp = buff.readLine();
					String res = "";
					if(temp == null) {
						throw new EOFException();	//indicates the end of the file
					}
					temp = temp.toLowerCase();
					
					//checks for any illegal characters per line
					for(int i = 0; i < temp.length(); i++) {
						char c = temp.charAt(i);
						if((c >= 'a' && c <= 'z') || c == ' ') {
							//correcting the spacing: skips a space if:
							//	res is not empty	 c = ' '	and	the char before c is ' '				  this is not the last char & the next char is ' '				  last char in sentence is ' '				
							if((res.length() > 0 && (c == ' ' && res.charAt(res.length() - 1) == ' ')) || (c == ' ' && i < temp.length() - 1 && temp.charAt(i + 1) == ' ') || (i == temp.length() - 1 && c == ' ') || (c == ' ' && i == 0))
								continue;
							res += c;
							startOfSentence = false;
						}
						else if(c == (char)39)
							continue;
						else {
							if(startOfSentence && temp.charAt(i + 1) == ' ') {
								startOfSentence = false;
								i++;
								continue;
							}
							//adds space if needs to, in case we replace a character separating 2 words
							if((i < temp.length() - 1 && temp.charAt(i + 1) != ' ') && (res.length() > 0 && res.charAt(res.length() - 1) != ' '))
								res += ' ';
						}
					}
					
					res += System.lineSeparator();
					fw.write(res);
				}
			} catch(EOFException e) {}
			
			buff.close();
			fw.close();
			
			//===============CLEAN===============
			//creates a new clean file
			File cleanFile = new File(cleanFilePath);
			cleanFile.createNewFile();
			//loads stopWords
			File stopFile = new File(STOP_WORDS_PATH);
			
			fr = new FileReader(stopFile);
			buff = new BufferedReader(fr);
			
			fw = new FileWriter(cleanFile);
			//putting all of the words in stopWords.txt into an array list
			try {
				String temp = "";
				while(temp != null) {
					temp = buff.readLine();
					this.stopWords.add(temp);
				}
			} catch(EOFException e) {}
			
			buff.close();
			
			normalizedFile = new File(normalizedFilePath);
			
			fr = new FileReader(normalizedFile);
			buff = new BufferedReader(fr);
			
			try {
				while(true) {
					String tempLine = buff.readLine();
					String tempWord = "";
					String res = "";
					
					if(tempLine == null) break;
					
					for(int i = 0; i < tempLine.length(); i++) {
						char c = tempLine.charAt(i);
						if(c != ' ') tempWord += c;
						else {
							if(!stopWords.contains(tempWord)) {
								res += tempWord;
								if(i < tempLine.length() - 1)
									res += ' ';
							}
							tempWord = "";
						}
					}
					if(!stopWords.contains(tempWord))
						res += tempWord;
					
					res += System.lineSeparator();
					fw.write(res);
				}
			} catch(EOFException e) {}
			
			buff.close();
			fw.close();
			
		} catch(IOException e) { 
			//deletes clean file
			File cleanFile = new File(cleanFilePath);
			cleanFile.delete();
			//deletes normalized file
			File normalizedFile = new File(normalizedFilePath);
			normalizedFile.delete();
			
			throw new CleanFailException(); 
		}
		finally {
			//closes any unclosed data streams
			if(buff != null) {
				try { buff.close(); }
				catch(IOException e) { throw new CleanFailException(); }
			}
			if(fw != null) {
				try { fw.close(); }
				catch(IOException e) { throw new CleanFailException(); }
			}
		}
	}

	public void countWords() throws CountFailException {
		//makes sure the clean file exists
		File cleanFile = new File(cleanFilePath);
		if(!cleanFile.exists()) throw new CountFailException();
		
		wordsMap = new HashMap<>();
		
		//declaring to be able to close them later
		FileReader fr = null;
		BufferedReader buff = null;
		
		try {
			fr = new FileReader(cleanFile);
			buff = new BufferedReader(fr);
			
			String temp = "";
			
			try {
				while(temp != null) {
					temp = buff.readLine();
					if(temp == null) throw new EOFException();
					
					String[] words = temp.split(" ");
					
					for(int i = 0; i < words.length; i++) {
						if(wordsMap.containsKey(words[i]))
							for(Map.Entry<String, Integer> entry : wordsMap.entrySet()) {
								String key = entry.getKey();
								int val = entry.getValue();
								
								if(words[i].equals(key)) {
									val++;
									entry.setValue(val);
								}
							}
						else if (!words[i].equals(""))
							wordsMap.put(words[i], 1);
					}
				}
			} catch(EOFException e) {}
			System.out.println(wordsMap.toString());
			buff.close();
		} catch(IOException e) {
			
		}
	}
	
	public void generateStatistics() throws GenerationFailException {
		try { countWords(); }
		catch(CountFailException e) { throw new GenerationFailException(); }
		
		String statFilePath = this.filePath.substring(0, filePath.length() - FILE_ENDING) + "Stat.txt";
		
		//declaring to be able to close them later
		FileReader fr = null;
		BufferedReader buff = null;
		FileWriter fw = null;
		
		//sets a hashmap with all the letters in the alphabet
		HashMap<Character, Integer> wordsPerLetter = new HashMap<>();
		for(char c = 'a'; c <= 'z'; c++)
			wordsPerLetter.put(c, 0);
		
		//amount of different words
		int numOfDiffWords = wordsMap.size();
		//amount of total words
		int numOfWords = 0;
		//most common words list
		ArrayList<String> commonWords = new ArrayList<>();
		//result
		String res = "";
		try {
			//load clean file
			File cleanFile = new File(cleanFilePath);
			//create stat file
			File statFile = new File(statFilePath);
			statFile.createNewFile();
			
			fw = new FileWriter(statFile);
			
			//go through all the words in the text to calculate statistics
			for(Map.Entry<String, Integer> entry : wordsMap.entrySet()) {
				//get each word and how many times they appear
				String wordKey = entry.getKey();
				int wordVal = entry.getValue();
				
				//go through every letter in the alphabet from the hashmap to find out which letter the current word starts with
				for(Map.Entry<Character, Integer> letterEntry : wordsPerLetter.entrySet()) {
					//get every character and how many words are starting with it
					char letterKey = letterEntry.getKey();
					int letterVal = letterEntry.getValue();
					//if the current word starts with this letter, increase this letter's value in the hashmap
					if(wordKey.charAt(0) == letterKey) {
						letterVal++;
						letterEntry.setValue(letterVal);
						}
				}
			

				//saves the most common words in an array list:
				//if the list is empty, add the first word you see to it
				if(commonWords.size() == 0)
					commonWords.add(wordKey);
				//if the current word appears the same number of times in the file, add the word to the list
				else if(wordVal == wordsMap.get(commonWords.get(0)))
					commonWords.add(wordKey);
				//if the current word appears more than the words in the array, clears the array and put the current word there
				else if (wordVal > wordsMap.get(commonWords.get(0))) {
					commonWords.clear();
					commonWords.add(wordKey);
				}
				//total number of words
				numOfWords += wordVal;
				
			}
			
			fw.write("Total number of words: " + numOfWords + System.lineSeparator());

			fw.write(System.lineSeparator());
			
			fw.write("Number of different words: " + numOfDiffWords + System.lineSeparator());

			fw.write(System.lineSeparator());
			
			//writes in file the most common word(s)
			String commonRes = "";
			for(String s: commonWords) {
				if(commonRes.equals(""))
					commonRes += s;
				else
					commonRes += ", " + s;
			}
			fw.write("Most common word(s): " + commonRes + System.lineSeparator());
			
			fw.write(System.lineSeparator());
			
			fw.write("Number of words starting with :" + System.lineSeparator());
			final int DIFF = 32;
			for(Map.Entry<Character, Integer> letterEntry : wordsPerLetter.entrySet()) {
				char letter = letterEntry.getKey();
				int numOfWordsPerLetter = letterEntry.getValue();
				fw.write((char)(letter - DIFF) + ": " + numOfWordsPerLetter + System.lineSeparator());
			}
			
			//==========Words Stat List==========
			
			fw.write(System.lineSeparator());
			
			TreeMap<String, Integer> sorted = new TreeMap<>();
			sorted.putAll(wordsMap);
			
			int longestWordLength = findLongestWord(wordsMap).length();
			
			String word = "Word";
			String wordAppearances = "Appearances";
			String wordPercent = "Text %";
			int tab = (longestWordLength/2);
			//styling the title line
			String titleFormat = "%" + tab + "s" + "%" + (tab + tab/2) + "s" + "%" + tab + "s";
			
			String title = String.format(titleFormat, word, wordAppearances, wordPercent);
			
			fw.write(title);
			fw.write(System.lineSeparator());
			
			String line = "";
			for(int i = 0; i < title.length() + tab; i++)
				line += "=";
			
			fw.write(line);
			fw.write(System.lineSeparator());
			
			String format = "%-" + longestWordLength + "s %-10d %f";
			
			for(Map.Entry<String, Integer> entry : sorted.entrySet()) {
				String wordKey = entry.getKey();
				int wordVal = entry.getValue();
				
				//percent per word
				double percent = (double)wordVal / (double)numOfWords * 100;
				//	   word		how many times it appears	% of appearance
				fw.write(String.format(format, wordKey, wordVal, percent) + " %" + System.lineSeparator());
			}
		}
		catch(IOException e) { throw new GenerationFailException(); }
	}
	
	private String findLongestWord(HashMap<String, Integer> map) {
		String res = "";
		for(Map.Entry<String, Integer> entry : map.entrySet()) {
			String key = entry.getKey();
			if(key.length() > res.length())
				res = key;
		}
		
		return res;
	}
}
