import java.awt.Color;
import java.awt.Graphics;

public class JapanFlag implements Drawable {

	@Override
	public void draw(int x, int y, int width, int height, Graphics g) {
		//drawing the japanese flag
		g.setColor(Color.WHITE);
		g.fillRect(x, y, width, height);
		
		g.setColor(Color.RED);
		
		int circleWidth = height / 2;
		int circleX = x + width / 2 - circleWidth / 2;
		int circleY = y + height / 2 - circleWidth / 2;
		g.fillOval(circleX, circleY, circleWidth, circleWidth);
	}

}
