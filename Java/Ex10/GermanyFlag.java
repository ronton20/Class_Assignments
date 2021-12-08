import java.awt.Color;
import java.awt.Graphics;

public class GermanyFlag implements Drawable {

	@Override
	public void draw(int x, int y, int width, int height, Graphics g) {
		//drawing the german flag
		int rectHeight = height / 3;
		g.setColor(Color.BLACK);
		g.fillRect(x, y, width , rectHeight);
		
		g.setColor(Color.RED);
		g.fillRect(x, y + rectHeight, width, rectHeight);
		
		g.setColor(Color.YELLOW);
		g.fillRect(x, y + 2 * rectHeight, width, rectHeight);
	}

}
