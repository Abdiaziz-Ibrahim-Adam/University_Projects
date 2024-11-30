import org.junit.jupiter.api.Test;

/**
 * Test class for Position klass {@link Position}.
 */
public class PositionTest {

    /**
     * Tests the equals method of the Position class.
     * Verifies that two positions with the same coordinates are considered equal.
     */
    @Test
    void equalsTest(){
        Position pos1 = new Position(3,4);
        Position pos2 = new Position(3,4);

        assert (pos1.equals(pos2));
    }

    /**
     * Tests the getX method of the Position class.
     * Verifies that the correct X-coordinate is returned.
     */
    @Test
    void getXTest(){
        Position pos = new Position(5,7);
        assert (pos.getX() == 5);
    }

    /**
     * Tests the getY method of the Position class.
     * Verifies that the correct Y-coordinate is returned.
     */
    @Test
    void getYTest(){
        Position pos = new Position(5,6);
        assert (pos.getY() == 6);
    }

    /**
     * Tests the getPosToNorth method of the Position class.
     * Verifies that the correct position to the north is returned.
     */
    @Test
    void getPosToNorthTest(){
        Position pos = new Position(5,8);
        Position NorthPos = pos.getPosToNorth();

        assert (NorthPos.getY() == 7);
    }

    /**
     * Tests the getPosToSouth method of the Position class.
     * Verifies that the correct position to the south is returned.
     */
    @Test
    void getPosToSouthTest(){
        Position pos = new Position(9,1);
        Position SouthPos = pos.getPosToSouth();
        assert (SouthPos.getY() == 2);
    }

    /**
     * Tests the getPosToEast method of the Position class.
     * Verifies that the correct position to the east is returned.
     */
    @Test
    void getPosToEastTest(){
        Position pos = new Position(3,5);
        Position EastPos = pos.getPosToEast();
        assert (EastPos.getX() == 4 );
    }

    /**
     * Tests the getPosToWest method of the Position class.
     * Verifies that the correct position to the west is returned.
     */
    @Test
    void getPosToWestTest(){
        Position pos = new Position(1,3);
        Position WestPos = pos.getPosToWest();
        assert (WestPos.getX() == 0);
    }

    /**
     * Tests the hashCode method of the Position class.
     * Verifies that two positions with the same coordinates have the same hash code.
     */
    @Test
    void hashcodeTest(){
        Position pos1 = new Position(7,4);
        Position pos2 = new Position(7,4);
        assert (pos1.hashCode() == pos2.hashCode());
    }

    /**
     * Tests the toString method of the Position class.
     * Verifies that the correct string representation of the position is returned.
     */
    @Test
    void ToStringTest(){
        Position pos = new Position(1,5);
        String Expected = "(1,5)";
        assert Expected.equals(pos.toString());
    }

}
