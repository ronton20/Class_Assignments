//Ron Amar 318479458
public class DnaSequence 
{
	private String sequence;
	private int[] nucleotideCounts;
	
	
	private void updateCount()									//updates nucleotideCounts
	{
		nucleotideCounts= new int[] {0,0,0,0};
		updateCount(0);
	}
	
	private void updateCount(int i)
	{
		if(i==sequence.length())
			return;
		
		if(sequence.charAt(i)=='A' || sequence.charAt(i)=='a')
			nucleotideCounts[0]++;
		if(sequence.charAt(i)=='C' || sequence.charAt(i)=='c')
			nucleotideCounts[1]++;
		if(sequence.charAt(i)=='G' || sequence.charAt(i)=='g')
			nucleotideCounts[2]++;
		if(sequence.charAt(i)=='T' || sequence.charAt(i)=='t')
			nucleotideCounts[3]++;
		
		updateCount(i+1);
		return;
	}
	
	
	private static boolean isCharValid(char c)					//checks if a character is A C G or T
	{
		return (c=='A' || c=='a' || c=='C' || c=='c' || c=='G' || c=='g' || c=='T' || c=='t');
	}
	
	private static boolean isStringValid(String s)				//checks if a string is null or empty
	{
		if(s==null)
			return false;
		if(s.length()==0)
			return false;
		return true;
	}
	
	private static boolean isDnaValid(String seq)				//checks if a sequence is valid
	{
		if(!isStringValid(seq))
			return false;
		return isDnaValid(seq, 0);
	}
	
	private static boolean isDnaValid(String seq, int i)
	{
		if(i==seq.length())
			return true;
		if(!isCharValid(seq.charAt(i)))
			return false;
		return (isCharValid(seq.charAt(i)) && isDnaValid(seq, i+1));
	}
	
	
	public DnaSequence()										//Default constructor
	{
		sequence="ACGT";
		updateCount();
	}
	
	public DnaSequence(String sequence)							//Constructor
	{
		if(isDnaValid(sequence))
			this.sequence=sequence;
		else
			this.sequence="ACGT";
		updateCount();
	}
	
	public DnaSequence(DnaSequence other)						//Copy constructor
	{
		this.sequence=other.sequence;
		updateCount();
	}
	
	public String getSequence()
	{
		return sequence;
	}
	
	public void setSequence(String sequence)
	{
		if(isDnaValid(sequence))		//makes sure the inputed sequence is valid
			this.sequence=sequence;
		updateCount();
	}
	
	public int[] getNucleotideCounts()
	{
		int[] ans=new int[4];
		ans[0]=nucleotideCounts[0];
		ans[1]=nucleotideCounts[1];
		ans[2]=nucleotideCounts[2];
		ans[3]=nucleotideCounts[3];
		return ans;
	}
	
	public static boolean isDnaSequence(String sequence)
	{
		return isDnaValid(sequence);
	}
	
	public void changeCapitalization()							//changes the capitalization of a DNA sequence
	{
		String s=sequence;
		sequence="";
		changeCapitalization(0, s);
	}
	
	private void changeCapitalization(int i, String s)
	 {
		 final int DIFF=32;
		 if(i==s.length())
			 return;
		 char c=s.charAt(i);
		 
		 if(c>='A' && c<='Z')
			 c+=DIFF;
		 else
			 c-=DIFF;
		 sequence+=c;
		 changeCapitalization(i+1, s);
		 return;
	 }
	
	public static boolean addArray(int[] arr1, int[] arr2)		//combines the values of 2 arrays
	{
		if(arr1==null || arr2==null)
			return false;
		if(arr1.length!=arr2.length)
			return false;
		return addArray(arr1, arr2, 0);
	}
	
	private static boolean addArray(int[] arr1, int[] arr2, int i)
	{
		if(i==arr1.length)
			return true;
		arr1[i]+=arr2[i];
		return addArray(arr1, arr2, i+1);
	}
	
	public void concatGenes(DnaSequence other)					//combines 2 DNA sequences into 1
	{
		this.sequence+=other.sequence;
		if(addArray(this.getNucleotideCounts(), other.getNucleotideCounts()))
			return;
		else
			updateCount();
	}
	
	public String toString()									//<sequence>: (<#A>, <#C>, <#G>, <#T>)
	{
		return String.format("%s: (%s, %s, %s, %s)", sequence, nucleotideCounts[0], nucleotideCounts[1], nucleotideCounts[2], nucleotideCounts[3]);
	}
}
