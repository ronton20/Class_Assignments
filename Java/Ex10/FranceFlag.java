
import java.awt.*;

public class FranceFlag implements Drawable
{
	public void draw(int x, int y, int width, int height, Graphics g) {
		//drawing the french flag
		int rectWidth = width / 3;
		g.setColor(Color.BLUE);
		g.fillRect(x, y, rectWidth, height);
		
		g.setColor(Color.WHITE);
		g.fillRect(x + rectWidth, y, rectWidth, height);
		
		g.setColor(Color.RED);
		g.fillRect(x + 2 * rectWidth, y, rectWidth, height);
	}
}
