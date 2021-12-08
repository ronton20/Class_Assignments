
public class Testing {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		DnaSequence dna1=new DnaSequence("AAcGttCa");
		int[] counts=dna1.getNucleotideCounts();
		System.out.println(dna1);
		
		System.out.println();
		
		DnaSequence dna2=new DnaSequence(dna1);
		int[] counts2=dna2.getNucleotideCounts();
		System.out.println(dna2);
		
		System.out.println();
		
		dna1.setSequence("AAtg");
		counts=dna1.getNucleotideCounts();
		System.out.println(dna1);
		
		System.out.println();
		
		System.out.println(dna2);
		
		System.out.println();
		
		dna2.changeCapitalization();
		System.out.println(dna2);
		
		System.out.println();
		
		dna1.setSequence("ATGtGtGttaAc");
		System.out.println(dna1);
		
		int[] gene3=dna1.getNucleotideCounts();
		gene3[3]=7;
		for(int i=0; i<gene3.length; i++)
			System.out.print(gene3[i] + " ");
		System.out.println();
		System.out.println(dna1);
	}

}
