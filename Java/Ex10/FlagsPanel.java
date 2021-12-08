//Nmae: Ron Amar
//ID: 318479458

import java.awt.*;
import javax.swing.*;

public class FlagsPanel extends JPanel 
{
	private Drawable[][] flags;
	private int rows;
	private int cols;
	
	public FlagsPanel(int rows, int cols) {
		flags = new Drawable[rows][cols];
		this.rows = rows;
		this.cols = cols;
		
		//setting the flags order
		for(int row = 0; row < flags.length; row++)
			for(int col = 0; col < flags[row].length; col ++) {
				//picking a random flag
				int rand = (int)(Math.random() * 3);
				switch(rand) {
					
				case 0: flags[row][col] = new FranceFlag();
						break;
				case 1: flags[row][col] = new GermanyFlag();
						break;
				case 2: flags[row][col] = new JapanFlag();
						break;
				}
			}
	}
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		//getting the width and height of each flag
		int flagWidth = super.getWidth() / cols;
		int flagHeight = super.getHeight() / rows;
		
		//painting the flags on the screen
		for(int row = 0; row < flags.length; row++)
			for(int col = 0; col < flags[row].length; col++)
				flags[row][col].draw(col * flagWidth, row * flagHeight, flagWidth, flagHeight, g);
	}
}
