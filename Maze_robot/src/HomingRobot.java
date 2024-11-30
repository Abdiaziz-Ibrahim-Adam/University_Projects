import java.util.Stack;

/**
 * This Java class implements a robot that navigates through a maze with the goal of reaching the closest goal position.
 * The robot utilizes a homing strategy, moving towards positions that minimize the distance to the closest goal.
 * It maintains a stack to backtrack if it reaches a dead-end.
 * The robot marks visited positions to avoid revisiting them.
 *
 * @author Abdiaziz Ibrahim Adam
 * @version 25 april 2024
 */
public class HomingRobot implements Robot {
    private Position p;
    private Maze m;
    private boolean[][] Visited;
    private Stack<Position> path;

    /**
     * The constructor initializes the robot in the maze and sets up data structures.
     * It also marks the start position as visited.
     *
     * @param maze The maze in which the robot will operate, provided during instantiation.
     */
    HomingRobot(Maze maze){
        this.m = maze;
        this.p = m.getStart();
        this.Visited = new boolean[maze.getNumRows()][maze.getNumColumns()];
        this.path = new Stack<>();
        Visited[p.getY()][p.getX()] = true;
    }

    /**
     * This method moves the robot according to the homing strategy.
     * If a movable position that hasn't been visited is found, it moves to that position.
     * If no such position is found and the path stack is not empty, it backtracks to the previous position.
     */
    public void move() {
        //Find a pos to move to(not visited and movable):
        Position nextPos = getNextPosition(this.p);
        if(nextPos != null){
            path.push(this.p);
            this.p = nextPos;
            Visited[nextPos.getY()][nextPos.getX()] = true;
        } else if (!path.isEmpty()) {
            this.p = path.pop();
        }
    }

    /**
     * This method finds the next movable position with the lowest distance to the closest goal.
     *
     * @param p The current position of the robot.
     * @return The next position to move to, or null if no such position is found.
     */
    private Position getNextPosition(Position p){
        Position[] nextPos = {p.getPosToNorth(), p.getPosToEast(), p.getPosToSouth(), p.getPosToWest() };

        Position bestPos = null;
        int lowestDistance = Integer.MAX_VALUE;

        for(int i = 0; i<nextPos.length; i++){
            Position next = nextPos[i];
            if(m.isMovable(next) && !Visited[next.getY()][next.getX()]){
                int distance = m.distanceToClosestGoal(next);

                if(distance < lowestDistance){
                    bestPos = next;
                    lowestDistance = distance;
                }
            }
        }
        return bestPos;
    }

    /**
     * Retrieves the current position of the robot in the maze.
     *
     * @return The current position of the robot.
     */
    public Position getPosition(){
        return this.p;
    }

    /**
     * Checks if the robot has reached the goal in the maze.
     *
     * @return true if the goal is reached, otherwise false.
     */
    public boolean hasReachedGoal(){
        return m.isGoal(this.p);
    }

}
