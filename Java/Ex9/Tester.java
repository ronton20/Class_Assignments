import java.io.IOException;

public class Tester {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		String myFile = "beatles.txt";
		try { 
			TextAnalyzer txtAn = new TextAnalyzer(myFile);
			txtAn.cleanText();
			txtAn.countWords();
			txtAn.generateStatistics();
		}
		catch(IOException e) { System.out.println("Illegal file path"); }
		catch(CleanFailException e) { System.out.println("Clean fail"); }
		catch(CountFailException e) { System.out.println("Count fail"); }
		catch(GenerationFailException e) { System.out.println("Generation fail"); }
	}
}
