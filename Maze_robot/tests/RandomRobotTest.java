import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.util.Scanner;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Test class for RandomRobot klass {@link RandomRobot}.
 */
public class RandomRobotTest {
    private Maze maze;
    private RandomRobot robot;

    /**
     * Tests the move method of the RandomRobot class.
     * Verifies that the robot moves to a new position after calling the move method.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void move() throws IOException {
        String mazeLayout =
                """
                        ************
                        * S        *
                        * ******** *
                        *    *   * *
                        *** **** * *
                        **     *G  *
                        ************
                        """;
        maze = new Maze(new Scanner(mazeLayout));
        robot = new RandomRobot(maze);

        Position initialPos = robot.getPosition();

        robot.move();

        Position nextPos = robot.getPosition();

        assertNotEquals(initialPos, nextPos);
    }

    /**
     * Tests the getPosition method of the RandomRobot class.
     * Verifies that the getPosition method returns the correct position of the robot.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void getPositionTest() throws IOException {
        String mazeLayout =
                """
                        ************
                        * S        *
                        * ******** *
                        *    *   * *
                        *** **** * *
                        **     *G  *
                        ************
                        """;

        maze = new Maze(new Scanner(mazeLayout));
        robot = new RandomRobot(maze);

        assertEquals (new Position(2,1),robot.getPosition(),"The Start coordinates is (2,1) So I should get those coordinates!");
    }

    /**
     * Tests the hasReachedGoal method of the RandomRobot class.
     * Verifies that the robot correctly detects reaching the goal.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void HasReachedGoal() throws IOException {
        String mazeLayout =
                """
                        ************
                        * S        *
                        * ******** *
                        *    *   * *
                        *** **** * *
                        **     *G  *
                        ************
                        """;

        maze = new Maze(new Scanner(mazeLayout));
        robot = new RandomRobot(maze);

        while(!robot.hasReachedGoal()){
            robot.move();
        }
        assert (robot.hasReachedGoal());
    }
}
