/*
 * Copyright by Ruman Gerst
 * Research Group Applied Systems Biology - Head: Prof. Dr. Marc Thilo Figge
 * https://www.leibniz-hki.de/en/applied-systems-biology.html
 * HKI-Center for Systems Biology of Infection
 * Leibniz Institute for Natural Product Research and Infection Biology - Hans Knöll Insitute (HKI)
 * Adolf-Reichwein-Straße 23, 07745 Jena, Germany
 *
 * This code is licensed under BSD 2-Clause
 * See the LICENSE file provided with this code for the full license.
 */

package org.hkijena.misa_imagej.utils;

import java.awt.*;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class MathUtils {

    private MathUtils() {

    }

    public static Point getLineIntersection(Line2D.Double pLine1, Line2D.Double pLine2) {
        Point result = null;

        double s1_x = pLine1.x2 - pLine1.x1;
        double s1_y = pLine1.y2 - pLine1.y1;

        double s2_x = pLine2.x2 - pLine2.x1;
        double s2_y = pLine2.y2 - pLine2.y1;

        double s = (-s1_y * (pLine1.x1 - pLine2.x1) + s1_x * (pLine1.y1 - pLine2.y1)) / (-s2_x * s1_y + s1_x * s2_y);
        double t = (s2_x * (pLine1.y1 - pLine2.y1) - s2_y * (pLine1.x1 - pLine2.x1)) / (-s2_x * s1_y + s1_x * s2_y);

        if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
            // Collision detected
            result = new Point(
                    (int) (pLine1.x1 + (t * s1_x)),
                    (int) (pLine1.y1 + (t * s1_y)));
        }   // end if

        return result;
    }

    public static Point getLineRectableIntersection(Line2D.Double line, Rectangle2D rectangle) {
        Point i1 = getLineIntersection(line, new Line2D.Double(rectangle.getX(), rectangle.getY(), rectangle.getX(), rectangle.getMaxY()));
        Point i2 = getLineIntersection(line, new Line2D.Double(rectangle.getX(), rectangle.getMaxY(), rectangle.getMaxX(), rectangle.getMaxY()));
        Point i3 = getLineIntersection(line, new Line2D.Double(rectangle.getMaxX(), rectangle.getMaxY(), rectangle.getMaxX(), rectangle.getY()));
        Point i4 = getLineIntersection(line, new Line2D.Double(rectangle.getMaxX(), rectangle.getY(), rectangle.getX(), rectangle.getY()));

        List<Point> results = new ArrayList<>();
        if(i1 != null)
            results.add(i1);
        if(i2 != null)
            results.add(i2);
        if(i3 != null)
            results.add(i3);
        if(i4 != null)
            results.add(i4);

        Point2D.Double reference = new Point2D.Double(line.getX1(), line.getY1());
        results.sort(Comparator.comparingDouble(reference::distanceSq));
        if(results.isEmpty())
            return null;
        else
            return results.get(0);
    }
}
