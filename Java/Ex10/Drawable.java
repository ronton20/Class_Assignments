import java.awt.Graphics;

public abstract interface Drawable 
{
	//to get a unified draw method
	public abstract void draw(int x, int y, int width, int height, Graphics g);
}
