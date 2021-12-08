
public class LimitedSentence 
{
	private String sentence;
	private char delimiter;
	
	/**
	 * Constructors:
	 */
	public LimitedSentence()						//default constructor
	{
		sentence="";
		delimiter=' ';
	}
	
	public LimitedSentence(String sentence, char delimiter)
	{
		if(isDelimiterValid(delimiter))				//checks if the delimiter recived is valid
			this.delimiter=delimiter;
		else
			this.delimiter=' ';
		
		if(isSentenceValid(sentence, delimiter))	//checks if the sentence recived is valid
			this.sentence=sentence;
		else
			this.sentence="";
	}
	
	public LimitedSentence(char delimiter)
	{
		if(isDelimiterValid(delimiter))
			this.delimiter=delimiter;
		else
			this.delimiter=' ';
		this.sentence="";
	}
	
	public LimitedSentence(LimitedSentence other)	//copy constructor
	{
		this.delimiter=other.delimiter;
		this.sentence=other.sentence;
	}
	
	/** getters:
	 * 
	 * @return the parameters stored
	 */
	public String getSentence() {
		return sentence;
	}

	public char getDelimiter() {
		return delimiter;
	}

	/**
	 * checks if the delimiter is valid
	 * @param delimiter delimiter
	 * @return true if not a letter
	 */
	private boolean isDelimiterValid(char delimiter)
	{
		if((delimiter>='a' && delimiter<='z') || (delimiter>='A' && delimiter<='Z'))
			return false;
		return true;
	}
	
	/**
	 * checks if the sentence is valid based on its contents and if the delimiter is valid
	 * @param sentence sentence
	 * @param delimiter delimiter
	 * @return true if the sentence is valid and can be used
	 */
	private boolean isSentenceValid(String sentence, char delimiter)
	{
		final int MAX_LENGTH=25;
		if(sentence.length()>MAX_LENGTH)
			return false;
		if(!isDelimiterValid(delimiter))
			return false;
		if(sentence.length()>0)
		{
			if(sentence.charAt(0)==delimiter || sentence.charAt(sentence.length()-1)==delimiter)	//checks if the sentence doesnt begin or ends with a delimiter
				return false;
			for(int i=0; i<sentence.length(); i++)		//checks if the sentence contains only letters and delimiters and no other characters
				if((sentence.charAt(i)>='a' && sentence.charAt(i)<='z')==false && (sentence.charAt(i)>='A' && sentence.charAt(i)<='Z')==false && (sentence.charAt(i)!=delimiter))
					return false;
			for(int i=0; i<sentence.length()-1; i++)
				if(sentence.charAt(i)==delimiter && sentence.charAt(i+1)==delimiter)
					return false;
			return true;
		}
		return true;
	}
	
	public char getFirstLetter()	//returns the first letter of the sentence
	{
		if(sentence!="")
			return sentence.charAt(0);
		return 0;
	}
	
	public char getMiddleLetter()	//returns the middle letter of the sentence
	{
		if(sentence!="")
			return sentence.charAt(sentence.length()/2);
		return 0;
	}
	
	public char getLastLetter()		//returns the last letter of the sentence
	{
		if(sentence!="")
			return sentence.charAt(sentence.length()-1);
		return 0;
	}
	
	/**
	 * checks if a String entered contains only letters
	 * @param word word
	 * @return true if word contains only letters
	 */
	private boolean isWord(String word)
	{
		for(int i=0; i<word.length(); i++)
			if(!(word.charAt(i)>='a' && word.charAt(i)<='z') && !(word.charAt(i)>='A' && word.charAt(i)<='Z'))
				return false;
		return true;
	}
	
	/**
	 * adds a word to the end of the sentence and checks if the word added is viable
	 * @param toAppend word
	 * @return true if the sentence recived after adding toAppend is valid
	 */
	public boolean appendWordEnd(String toAppend)
	{
		if(!isWord(toAppend))
			return false;
		
		String temp;
		if(sentence=="")
			temp=toAppend;
		else
			temp=this.sentence + "" + delimiter + "" +toAppend;
		if(isSentenceValid(temp, delimiter))
		{
			this.sentence=temp;
			return true;
		}
		return false;
	}
	
	/**
	 * adds a word to the start of the sentence and checks if the word added is viable
	 * @param toAppend word
	 * @return true if the sentence recived after adding toAppend is valid
	 */
	public boolean appendWordFront(String toAppend)
	{
		if(!isWord(toAppend))
			return false;
		
		String temp;
		if(sentence=="")
			temp=toAppend;
		else
			temp=toAppend + "" + delimiter + "" +sentence;
		
		if(isSentenceValid(temp, delimiter))
		{
			this.sentence=temp;
			return true;
		}
		return false;
	}
	
	/**
	 * counts how many words are in a given sentence
	 * @return the number of words in the sentence
	 */
	private int wordAmmount()
	{
		int sum=0;
		if(sentence.length()>0)
			sum++;
		for(int i=0; i<sentence.length(); i++)
		{
			if(sentence.charAt(i)==delimiter)
				sum++;
		}
		return sum;
	}
	
	/**
	 * returns the Ith word in a sentence
	 * @param i word number
	 * @return the Ith word (if exists), if the word doesnt exist returns an empty string.
	 */
	public String getIthWord(int i)
	{
		int words=wordAmmount();
		if(i>words-1 || i<0)
			return "";
		
		int currentWord=0;
		String ans="";
		for(int j=0; j<sentence.length(); j++)
		{
			if(sentence.charAt(j)==delimiter)
			{
				currentWord++;
			}
			if(currentWord==i && sentence.charAt(j)!=delimiter)
				ans+=sentence.charAt(j);
		}
		return ans;
	}
	
	/**
	 * returns the Ith word in a sentence but in reversed order
	 * @param i word number
	 * @return reversed Ith word 
	 */
	public String getIthWordReversed(int i)
	{
		String temp=getIthWord(i);
		String rev="";
		
		for(int j=temp.length()-1; j>=0; j--)
		{
			rev+=temp.charAt(j);
		}
		return rev;
	}
	
	public int countWords()		//same as wordAmmount but public.
	{
		return wordAmmount();
	}
	
	/**
	 * Flips upper and lower case letters of the sentence
	 */
	public void reverseUpperLower()
	{
		final int DIFF=32;
		String temp="";
		for(int i=0; i<this.sentence.length(); i++)
		{
			if(this.sentence.charAt(i)>='A' && this.sentence.charAt(i)<='Z')
				temp+=""+(char)(this.sentence.charAt(i)+DIFF);
			else if(this.sentence.charAt(i)>='a' && this.sentence.charAt(i)<='z')
				temp+=""+(char)(this.sentence.charAt(i)-DIFF);
			else if(this.sentence.charAt(i)==delimiter)
				temp+=""+delimiter;
		}
		this.sentence=temp;
	}
	
	/**
	 * returns the sentence
	 */
	public String toString()
	{
		return sentence;
	}
}
