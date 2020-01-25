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

package org.hkijena.misa_imagej.ui.pipeliner;

import com.google.common.eventbus.Subscribe;
import org.hkijena.misa_imagej.api.pipelining.MISAPipeline;
import org.hkijena.misa_imagej.api.pipelining.MISAPipelineNode;
import org.hkijena.misa_imagej.utils.GraphicsUtils;
import org.hkijena.misa_imagej.utils.MathUtils;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Line2D;
import java.util.*;
import java.util.List;

public class MISAPipelineUI extends JPanel implements MouseMotionListener, MouseListener {

    private MISAPipeline pipeline;

    private MISAPipelineNodeUI currentlyDragged;
    private Point currentlyDraggedOffset = new Point();

    private Map<MISAPipelineNode, MISAPipelineNodeUI> nodeUIMap = new HashMap<>();
    private List<RemoveEdgeButton> removeEdgeButtonList = new ArrayList<>();

    public MISAPipelineUI() {
        super(null);
        initialize();
        refresh();
    }

    public MISAPipeline getPipeline() {
        return pipeline;
    }

    private void initialize() {
        setBackground(Color.WHITE);
        addMouseListener(this);
        addMouseMotionListener(this);
    }

    @Subscribe
    public void handleNodeUpdateEvent(MISAPipeline.AddedNodeEvent event) {
        refresh();
    }

    @Subscribe
    public void handleNodeUpdateEvent(MISAPipeline.AddedEdgeEvent event) {
        refresh();
    }

    @Subscribe
    public void handleNodeUpdateEvent(MISAPipeline.RemovedEdgeEvent event) {
        refresh();
    }

    @Subscribe
    public void handleNodeUpdateEvent(MISAPipeline.RemovedNodeEvent event) {
        refresh();
    }

    public void refresh() {
        nodeUIMap.clear();
        removeEdgeButtonList.clear();
        removeAll();
        if(pipeline != null) {
            for(MISAPipelineNode node : pipeline.getNodes()) {
                addNodeUI(node);

                // Add edges
                if(pipeline.getEdges().containsKey(node)) {
                    for(MISAPipelineNode target : pipeline.getEdges().get(node)) {
                        addEdgeUI(node, target);
                    }
                }

            }
        }
        if(getParent() != null)
            getParent().revalidate();
        repaint();
        updateEdgeUI();
    }

    public void setPipeline(MISAPipeline pipeline) {
        if(this.pipeline != null)
            this.pipeline.getEventBus().unregister(this);
        this.pipeline = pipeline;
        this.pipeline.getEventBus().register(this);
        refresh();
    }

    private void updateEdgeUI() {
        for(RemoveEdgeButton button : removeEdgeButtonList) {
            if(currentlyDragged != null) {
                button.setVisible(false);
            }
            else {
                button.setVisible(true);
                MISAPipelineNodeUI sourceUI = nodeUIMap.get(button.source);
                MISAPipelineNodeUI targetUI = nodeUIMap.get(button.target);

                int x1 = sourceUI.getX() + sourceUI.getWidth() / 2;
                int y1 = sourceUI.getY() + sourceUI.getHeight() / 2;
                int x2 = targetUI.getX() + targetUI.getWidth() / 2;
                int y2 = targetUI.getY() + targetUI.getHeight() / 2;
                int xc = x1 + (x2 - x1) / 2;
                int yc = y1 + (y2 - y1) / 2;
                button.setLocation(xc - button.getWidth() / 2, yc - button.getHeight() / 2);
            }
        }
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        super.paintComponent(graphics);

        Graphics2D g = (Graphics2D)graphics;
        RenderingHints rh = new RenderingHints(
                RenderingHints.KEY_TEXT_ANTIALIASING,
                RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g.setRenderingHints(rh);

        // Draw the edges
        graphics.setColor(Color.BLACK);
        g.setStroke(new BasicStroke(2));
        for(Map.Entry<MISAPipelineNode, Set<MISAPipelineNode>> kv : pipeline.getEdges().entrySet()) {
            for(MISAPipelineNode target : kv.getValue()) {
                MISAPipelineNodeUI sourceUI = nodeUIMap.get(kv.getKey());
                MISAPipelineNodeUI targetUI = nodeUIMap.get(target);
                Point targetPoint = MathUtils.getLineRectableIntersection(new Line2D.Double(sourceUI.getX() + sourceUI.getWidth() / 2.0, sourceUI.getY() + sourceUI.getHeight() / 2.0,
                        targetUI.getX() + targetUI.getWidth() / 2.0, targetUI.getY() + targetUI.getHeight() / 2.0),
                        new Rectangle(targetUI.getX(), targetUI.getY(), targetUI.getWidth(), targetUI.getHeight()));
                if(targetPoint != null) {
                    GraphicsUtils.drawArrowLine(g, sourceUI.getX() + sourceUI.getWidth() / 2,
                            sourceUI.getY() + sourceUI.getHeight() / 2, targetPoint.x, targetPoint.y, 8, 5);
                }
            }
        }
    }

    private MISAPipelineNodeUI addNodeUI(MISAPipelineNode node) {
        MISAPipelineNodeUI ui = new MISAPipelineNodeUI(node);
        add(ui);
        ui.setBounds(node.getX(), node.getY(), 200,150);
        nodeUIMap.put(node, ui);
        return ui;
    }

    private void addEdgeUI(MISAPipelineNode source, MISAPipelineNode target) {
        RemoveEdgeButton button = new RemoveEdgeButton(source, target);
        add(button);
        button.setBounds(0,0,21,21);
        removeEdgeButtonList.add(button);
    }

    @Override
    public void mouseDragged(MouseEvent mouseEvent) {
        if(currentlyDragged != null) {
            currentlyDragged.setLocation(currentlyDraggedOffset.x + mouseEvent.getX(),
                    currentlyDraggedOffset.y + mouseEvent.getY());
            repaint();
            updateEdgeUI();
            if(getParent() != null)
                getParent().revalidate();
        }
    }

    @Override
    public void mouseMoved(MouseEvent mouseEvent) {

    }

    @Override
    public void mouseClicked(MouseEvent mouseEvent) {

    }

    @Override
    public void mousePressed(MouseEvent mouseEvent) {
        if(mouseEvent.getButton() == MouseEvent.BUTTON1) {
            for(int i = 0; i < getComponentCount(); ++i) {
                Component component = getComponent(i);
                if(component.getBounds().contains(mouseEvent.getX(), mouseEvent.getY())) {
                    if(component instanceof MISAPipelineNodeUI) {
                        currentlyDragged = (MISAPipelineNodeUI)component;
                        currentlyDraggedOffset.x = component.getX() - mouseEvent.getX();
                        currentlyDraggedOffset.y = component.getY() - mouseEvent.getY();
                        break;
                    }
                }
            }
        }
    }

    @Override
    public void mouseReleased(MouseEvent mouseEvent) {
        currentlyDragged = null;
        updateEdgeUI();
    }

    @Override
    public void mouseEntered(MouseEvent mouseEvent) {

    }

    @Override
    public void mouseExited(MouseEvent mouseEvent) {

    }

    @Override
    public Dimension getPreferredSize() {
        int width = 0;
        int height = 0;
        for(int i = 0; i < getComponentCount(); ++i) {
            Component component = getComponent(i);
            width = Math.max(width, component.getX() + component.getWidth());
            height = Math.max(height, component.getY() + component.getHeight());
        }
        return new Dimension(width, height);
    }

    private static class RemoveEdgeButton extends JButton {

        private MISAPipelineNode source;
        private MISAPipelineNode target;

        public RemoveEdgeButton(MISAPipelineNode source, MISAPipelineNode target) {
            super(UIUtils.getIconFromResources("remove.png"));
            this.source = source;
            this.target = target;
            addActionListener(actionEvent -> source.getPipeline().removeEdge(source, target));
        }

    }

}