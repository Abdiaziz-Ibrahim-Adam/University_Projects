/**
 * A class representing a position in a 2D grid
 * @author johane
 * @version 2024-03-18
 */
public class Position {
	private final int x;
	private final int y;
	
	/**
	 * Create a new position
	 * @param x the x coordinate of this position
	 * @param y the y coordinate of this position
	 */
	public Position(int x, int y) {
		super();
		this.x = x;
		this.y = y;
	}
	
	/**
	 * Compare to positions for equality
	 * @see Object#equals(Object)
	 */
	@Override
	public boolean equals(Object obj) {
		if(!(obj instanceof Position)) {
			return false;
		}
		Position p=(Position)obj;
		return p.x==x&&p.y==y;
	}
	
	/**
	 * The x part of the coordinate
	 * @return x part of coordinate
	 */
	public int getX() {
		return x;
	}
	
	/**
	 * The y part of the coordinate
	 * @return y part of coordinate
	 */
	public int getY() {
		return y;
	}

	/**
	 * Get the position to the north of this position
	 * @return the position to the north of this position
	 */
	public Position getPosToNorth(){
		return new Position(x,y-1);
	}

	/**
	 * Get the position to the south of this position
	 * @return the position to the south of this position
	 */
	public Position getPosToSouth(){
		return new Position(x,y+1);
	}

	/**
	 * Get the position to the west of this position
	 * @return the position to the west of this position
	 */
	public Position getPosToWest(){
		return new Position(x-1,y);
	}

	/**
	 * Get the position to the east of this position
	 * @return the position to the east of this position
	 */
	public Position getPosToEast(){
		return new Position(x+1,y);
	}
	
	/**
	 * @see Object#hashCode()
	 */
	@Override
	public int hashCode() {
		return x+y*1000;
	}

	@Override
	public String toString() {
		return "("+x+","+y+")";
	}

}
