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
import org.hkijena.misa_imagej.api.pipelining.MISAPipelineNode;
import org.hkijena.misa_imagej.ui.parametereditor.MISAModuleInstanceUI;
import org.hkijena.misa_imagej.utils.UIUtils;
import org.hkijena.misa_imagej.utils.ui.DocumentChangeListener;
import org.hkijena.misa_imagej.utils.ui.MonochromeColorIcon;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.event.DocumentEvent;
import java.awt.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

public class MISAPipelineNodeUI extends JPanel implements ComponentListener {

    private MISAPipelineNode node;
    private static final Color BORDER_COLOR = new Color(128,128,128);

    public MISAPipelineNodeUI(MISAPipelineNode node) {
        super(new BorderLayout());
        this.node = node;
        initialize();
    }

    private void initialize() {
        addComponentListener(this);

        JPanel padding = new JPanel(new BorderLayout(8,8));
        padding.setOpaque(false);
        padding.setBorder(BorderFactory.createEmptyBorder(16,8,8,8));

        // Create name editor
        JTextField nameEditor = new JTextField(node.getName()) {
            @Override
            public void setBorder(Border border) {
                // No border
            }
        };
        nameEditor.setBackground(this.getBackground());
        nameEditor.setFont(nameEditor.getFont().deriveFont(14.0f));
        nameEditor.getDocument().addDocumentListener(new DocumentChangeListener() {
            @Override
            public void changed(DocumentEvent documentEvent) {
                node.setName(nameEditor.getText());
            }
        });
        padding.add(nameEditor, BorderLayout.NORTH);

        // Create description editor
        JTextArea descriptionEditor = new JTextArea(node.getDescription());
        descriptionEditor.setBackground(getBackground());
        descriptionEditor.getDocument().addDocumentListener(new DocumentChangeListener() {
            @Override
            public void changed(DocumentEvent documentEvent) {
                node.setDescription(descriptionEditor.getText());
            }
        });
        padding.add(descriptionEditor, BorderLayout.CENTER);

        // Create the UI for functions
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.LINE_AXIS));

        JButton connectButton = new JButton(UIUtils.getIconFromResources("connect.png"));
        connectButton.setToolTipText("Connect from another node");
        initializeConnectMenu(UIUtils.addPopupMenuToComponent(connectButton), connectButton);
        UIUtils.makeFlat(connectButton);
        buttonPanel.add(connectButton);

        buttonPanel.add(Box.createHorizontalGlue());

        JButton removeNodeButton = new JButton(UIUtils.getIconFromResources("remove.png"));
        removeNodeButton.setToolTipText("Remove entry");
        removeNodeButton.addActionListener(actionEvent -> removeNode());
        UIUtils.makeFlat(removeNodeButton);
        buttonPanel.add(removeNodeButton);

        JButton editParametersButton = new JButton(UIUtils.getIconFromResources("edit.png"));
        editParametersButton.setToolTipText("Edit parameters");
        editParametersButton.addActionListener(actionEvent -> editParameters());
        UIUtils.makeFlat(editParametersButton);
        buttonPanel.add(editParametersButton);

        padding.add(buttonPanel, BorderLayout.SOUTH);
        add(padding, BorderLayout.CENTER);
    }



    private void initializeConnectMenu(JPopupMenu menu, JButton connectButton) {
        boolean addedItem = false;
        for(MISAPipelineNode available : node.getAvailableInNodes()) {
            JMenuItem menuItem = new JMenuItem("From " + available.getName(),
                    new MonochromeColorIcon(UIUtils.getIconFromResources("module-template.png"),
                            available.getModuleInstance().getModuleInfo().toColor()));
            menuItem.addActionListener(actionEvent -> {
                node.getPipeline().addEdge(available, node);
            });
            available.getEventBus().register(new Object() {
                @Subscribe
                public void handleNameChangeEvent(MISAPipelineNode.ChangedNameEvent event) {
                    menuItem.setText("From " + event.getNode().getName());
                }
            });
            menu.add(menuItem);
            addedItem = true;
        }

        if(!addedItem)
            connectButton.setVisible(false);
    }

    private void removeNode() {
        if(JOptionPane.showConfirmDialog(this, "Do you really want to remove the entry '" +
                node.getName() + "'?", "Remove entry", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE)
                == JOptionPane.YES_OPTION) {
            node.getPipeline().removeNode(node);
        }
    }

    private void editParameters() {
        MISAModuleInstanceUI editor = new MISAModuleInstanceUI(node.getModuleInstance(), true, false);
        editor.setTitle("MISA++ pipeline tool - Parameters for " + node.getName()
                + " (" + node.getModuleInstance().getModuleInfo().getId() + ")");
        editor.pack();
        editor.setSize(new Dimension(800,600));
        editor.setVisible(true);

        // Java separates between JFrame and JDialog
        // Both solutions are not good for out use case
        // Workaround: Disable parent frame
        JFrame pipeliner = (JFrame)SwingUtilities.getWindowAncestor(this);
        editor.addWindowListener(new WindowListener() {
            @Override
            public void windowOpened(WindowEvent windowEvent) {

            }

            @Override
            public void windowClosing(WindowEvent windowEvent) {

            }

            @Override
            public void windowClosed(WindowEvent windowEvent) {
                pipeliner.setEnabled(true);
            }

            @Override
            public void windowIconified(WindowEvent windowEvent) {

            }

            @Override
            public void windowDeiconified(WindowEvent windowEvent) {

            }

            @Override
            public void windowActivated(WindowEvent windowEvent) {

            }

            @Override
            public void windowDeactivated(WindowEvent windowEvent) {

            }
        });
        pipeliner.setEnabled(false);
    }

    public MISAPipelineNode getNode() {
        return node;
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        super.paintComponent(graphics);

        graphics.setColor(node.getModuleInstance().getModuleInfo().toColor());
        graphics.fillRect(0,0, getWidth() - 1, 8);
        graphics.setColor(BORDER_COLOR);
        graphics.drawRect(0,0,getWidth() - 1, getHeight() - 1);

    }


    @Override
    public void componentResized(ComponentEvent componentEvent) {

    }

    @Override
    public void componentMoved(ComponentEvent componentEvent) {
        node.setX(getX());
        node.setY(getY());
    }

    @Override
    public void componentShown(ComponentEvent componentEvent) {

    }

    @Override
    public void componentHidden(ComponentEvent componentEvent) {

    }
}
