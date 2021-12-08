/*
 * @Author: Ron Amar
 * ID:	318479458
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Random;

public class CatchMe extends JPanel 
{
	//starting values
	private final int STARTING_SIZE = 50;
	private final int STARTING_DELAY = 1500;
	private final Color STARTING_COLOR = Color.GREEN;
	private final int STARTING_ACCURACY = 100;
	private final int DEFAULT_WINDOW_SIZE = 500;
	
	private int blockSize;
	private int delay;
	private Color color;
	private int x;
	private int y;
	
	private JButton cmdRestart;
	private JLabel lblScore;
	//the format of the label
	private final String FORMAT = "Squares: %-5s" + "Clicks: %-6s" + "Score: %-5s" + "Accuracy: %.2f";
	private int score;
	private int tries;
	private int clicks;
	//shows the player his accuracy with the mouse
	private double accuracy;
	
	private Timer t;
	
	public CatchMe() 
	{
		
		delay = STARTING_DELAY;
		blockSize = STARTING_SIZE;
		score = 0;
		tries = 0;
		clicks = 0;
		accuracy = STARTING_ACCURACY;
		color = STARTING_COLOR;
		
		//=====LABEL=====
		//setting it for the first time
		lblScore = new JLabel();
		lblScore.setText(String.format(FORMAT, tries, clicks, score, accuracy));
		lblScore.setForeground(Color.WHITE);
		add(lblScore);
		
		//cant get the window sizes so for the first time it tries in a default window size;
		x = (int)(Math.random() * DEFAULT_WINDOW_SIZE);
		y = (int)(Math.random() * DEFAULT_WINDOW_SIZE);
		
		//setting up the timer with its action listener
		t = new Timer(delay, new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				//advances the difficulty of the game
				if((tries != 0 && tries % 5 == 0) && blockSize > 11) blockSize -= 2;
				if((tries != 0 && tries % 7 == 0) && delay > 9) delay -= 10;
				moveBlock();
				changeColor();
				//updating the label
				lblScore.setText(String.format(FORMAT, tries, clicks, score, accuracy));
				repaint();
			}
		});
		//=====MOUSE CLICKS=====
		//setting up the mouse adapter because we only use the mouse pressed function
		this.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				// TODO Auto-generated method stub
				super.mouseClicked(e);
				//checks if the person clicked on the square and gives him feedback (paints the square white)
				if(!color.equals(Color.WHITE)) {
					if((e.getX() >= x && e.getX() <= x + blockSize) && (e.getY() >= y && e.getY() <= y + blockSize)) {
						color = Color.WHITE;
						score++;
					}
					clicks++;
					updateAccuracy();
					lblScore.setText(String.format(FORMAT, tries, clicks, score, accuracy));
					repaint();
				}
			}
		});

		//=====BUTTON=====
		cmdRestart = new JButton("New Game");
		cmdRestart.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				//resetting all the values to their starting points when the reset button is pressed
				blockSize = STARTING_SIZE;
				delay = STARTING_DELAY;
				color = STARTING_COLOR;
				score = 0;
				tries = 0;
				clicks = 0;
				accuracy = 100;
				//updating the label accordingly
				lblScore.setText(String.format(FORMAT, tries, clicks, score, accuracy));
				moveBlock();
				changeColor();
				repaint();
			}
		});
		
		this.add(cmdRestart);
		
		t.start();
	}
	
	//a method to update the accuracy of the player's clicks
	private void updateAccuracy() {
		accuracy = (double)score / (double)clicks * 100;
	}
	
	//moves the block to a random new position and updates the counter of how many blocks there were
	private void moveBlock() {
		x = (int)(Math.random() * (this.getWidth() - blockSize));
		y = (int)(Math.random() * (this.getHeight() - blockSize));
		tries++;
	}
	
	//self explanatory
	private void changeColor() {
		Random rand = new Random();
		float r = rand.nextFloat();
		float g = rand.nextFloat();
		float b = rand.nextFloat();
		
		color = new Color(r, g, b);
	}
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		super.setBackground(Color.BLACK);
		//painting the square on the screen
		g.setColor(color);
		g.fillRect(x, y, blockSize, blockSize);
	}
}
