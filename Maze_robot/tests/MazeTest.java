import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.util.Scanner;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Test class for the Maze klass {@link Maze}.
 */
public class MazeTest {
    private Maze maze;

    /**
     * Tests the getNumColumns method of the Maze class.
     * Verifies that the correct number of columns is returned.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void getNumColumns() throws IOException {
        String mazeLayout =
                        "************\n" +
                        "* S        *\n" +
                        "* ******** *\n" +
                        "*    *   * *\n" +
                        "*** **** * *\n" +
                        "**     *G  *\n" +
                        "************\n";
        maze = new Maze(new Scanner(mazeLayout));

        assertEquals(12, maze.getNumColumns(), "Num of cols should be 12");
    }

    /**
     * Tests the getNumRows method of the Maze class.
     * Verifies that the correct number of rows is returned.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void getNumRows() throws IOException {
        String mazeLayout =
                        "************\n" +
                        "* S        *\n" +
                        "* ******** *\n" +
                        "*    *   * *\n" +
                        "*** **** * *\n" +
                        "**     *G  *\n" +
                        "************\n";

        maze = new Maze(new Scanner(mazeLayout));

        assertEquals(7, maze.getNumRows(), "Num of rows should be 7");
    }

    /**
     * Tests the isGoal method of the Maze class.
     * Verifies that the correct result is returned for a given position being a goal.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void isGoalTest() throws IOException {
        String mazeLayout =
        "*****\n" +
        "* S *\n" +
        "* ***\n" +
        "*   *\n" +
        "*** *\n" +
        "*G  *\n";

        maze = new Maze(new Scanner(mazeLayout));

        assertFalse(maze.isGoal(maze.getStart()));
        assertTrue(maze.isGoal(new Position(1,5)), "G coordinates is (1,5) so this should be true");
    }

    /**
     * Tests the isMovable method of the Maze class.
     * Verifies that the correct result is returned for a given position being movable.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void isMovableTest() throws IOException {
        String mazeLayout =
                        "*****\n" +
                        "* S *\n" +
                        "* ***\n" +
                        "*   *\n" +
                        "*** *\n" +
                        "*G  *\n";
        maze = new Maze(new Scanner(mazeLayout));

        assertTrue(maze.isMovable(new Position(1,1)));
        assertTrue(maze.isMovable(new Position(2,3)));
        assertFalse(maze.isMovable(new Position(3,2)));
        assertFalse(maze.isMovable(new Position(5,1)));
    }

    /**
     * Tests the distanceToClosestGoal method of the Maze class.
     * Verifies that the correct distance to the closest goal is returned from a given position.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void distanceClosestToGoalTest() throws IOException {
        String mazeLayout =
                        "************\n" +
                        "*S         *\n" +
                        "* ******** *\n" +
                        "*    *   * *\n" +
                        "*** **** * *\n" +
                        "**     *G  *\n" +
                        "************\n";

        maze = new Maze(new Scanner(mazeLayout));

        Position start = new Position(1,1);

        assertEquals(11,maze.distanceToClosestGoal(start));
    }

    /**
     * Tests the getStart method of the Maze class.
     * Verifies that the correct start position is returned.
     *
     * @throws IOException if an I/O error occurs while reading the maze layout.
     */
    @Test
    void getStartTest() throws IOException {
        String mazeLayout =
                        "************\n" +
                        "*S         *\n" +
                        "* ******** *\n" +
                        "*    *   * *\n" +
                        "*** **** * *\n" +
                        "**     *G  *\n" +
                        "************\n";

        maze = new Maze(new Scanner(mazeLayout));
        assertEquals(new Position(1,1), maze.getStart(), "Start pos have coordinates (1,1) in the maze");
    }
}
