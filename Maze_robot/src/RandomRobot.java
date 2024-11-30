import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * A robot that moves in a random direction
 * @author johane
 * @version 2024-03-18
 */
public class RandomRobot implements Robot  {
	protected Position pos;
	protected Maze m;
	Position oldPos;

	/**
	 * Construct a robot in a given maze.
	 * @param m the maze the robot should explore
	 */
	public RandomRobot(Maze m) {
		pos = m.getStart();
		this.m=m;
		oldPos=pos;
	}
	
	/**
	 * Move this robot. Tries to move in a random direction.
	 * Avoids going back from where it came if possible
	 */
	public void move() {
		ArrayList<Position> movable = getMovableNeighboursForward();
		if(movable.size()==0) {
			stepBack();
		}
		else {
			moveToRandom(movable);
		}
	}

	/**
	 * Get the neighbours of the current position that are movable.
	 * Excludes the position the robot came from
	 * @return
	 */
	private ArrayList<Position> getMovableNeighboursForward() {
		ArrayList<Position> movable = new ArrayList<>();
		List<Position> neighbours = getNeighbours();
		for(Position p:neighbours) {
			if(m.isMovable(p)&&!p.equals(oldPos))
				movable.add(p);
		}
		return movable;
	}

	/**
	 * Move to a random position from the list of movable positions
	 * @param movable the list of movable positions

	 */
	private void moveToRandom(ArrayList<Position> movable) {
		oldPos=pos;
		Position newPos=movable.get((int) (Math.random() * movable.size()));
		pos=newPos;
	}

	/**
	 * Step back to the previous position
	 */
	private void stepBack() {
		Position temp=pos;
		pos=oldPos;
		oldPos=temp;
	}

	/**
	 * Get the neighbours of the current position
	 * @return a list of the neighbours
	 */
	private ArrayList<Position> getNeighbours() {
		ArrayList<Position> neighbours = new ArrayList<>();
		neighbours.add(pos.getPosToNorth());
		neighbours.add(pos.getPosToSouth());
		neighbours.add(pos.getPosToEast());
		neighbours.add(pos.getPosToWest());
		return neighbours;
	}

	/**
	 * Get the current position of the robot
	 * @return the position
	 */
	public Position getPosition() {
		return pos;
	}

	/**
	 * Check if the robot has reached the goal
	 * @return true if the robot has reached the goal
	 */
	public boolean hasReachedGoal() {
		return m.isGoal(pos);
	}


}
