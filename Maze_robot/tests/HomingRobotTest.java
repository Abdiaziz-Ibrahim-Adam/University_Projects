import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.util.Scanner;
import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * Test class for {@link HomingRobot}.
 */
public class HomingRobotTest {
    private Maze maze;
    private HomingRobot robot;

    /**
     * Tests the move method of the HomingRobot class.
     * Verifies that the robot reaches the goal following the shortest path.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void moveTest() throws IOException {
        String mazeLayout =
                """
                        ********************
                        *S       *         *
                        ****** * ***** ****
                        * *     *   *      *
                        * * *** ***** *****
                        * * *         *
                        * *** ********* *****
                        *     *             *
                        ***G*****************""";

        maze = new Maze(new Scanner(mazeLayout));
        robot = new HomingRobot(maze);

        int shortestPath = 27;
        int counter = 0;

        while (!robot.hasReachedGoal()){
            robot.move();
            counter++;
        }
        assertEquals(counter,shortestPath);
    }

    /**
     * Tests the getPosition method of the HomingRobot class.
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
        robot = new HomingRobot(maze);

        assertEquals (new Position(2,1),robot.getPosition(),"The Start coordinates is (2,1) So I should get those coordinates!");
    }


    /**
     * Tests the hasReachedGoal method of the HomingRobot class.
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
        robot = new HomingRobot(maze);

        while(!robot.hasReachedGoal()){
            robot.move();
        }
        assert (robot.hasReachedGoal());
    }
}

