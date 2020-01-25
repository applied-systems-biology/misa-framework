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

package org.hkijena.misa_imagej.ui.components;

import org.hkijena.misa_imagej.utils.ResourceUtils;
import org.hkijena.misa_imagej.utils.UIUtils;

import javax.swing.*;
import javax.swing.text.BadLocationException;
import javax.swing.text.rtf.RTFEditorKit;
import java.awt.*;
import java.io.IOException;
import java.io.InputStream;

public class RTFReader extends JPanel {

    private JScrollPane scrollPane;
    private JTextPane content;
    private RTFEditorKit rtfEditorKit;

    public RTFReader() {
        initialize();
    }

    private void initialize() {
        setLayout(new BorderLayout());

        content = new JTextPane();
        content.setEditable(false);
//        content.addHyperlinkListener(e -> {
//            if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
//                if (e.getDescription() != null && e.getDescription().startsWith("#")) {
//                    SwingUtilities.invokeLater(() -> scrollToReference(e.getDescription().substring(1)));
//                } else {
//                    if (Desktop.isDesktopSupported()) {
//                        try {
//                            Desktop.getDesktop().browse(e.getURL().toURI());
//                        } catch (Exception e1) {
//                            throw new RuntimeException(e1);
//                        }
//                    }
//                }
//            }
//        });

        rtfEditorKit = new RTFEditorKit();

        content.setEditorKit(rtfEditorKit);
        content.setContentType("text/rtf");
        scrollPane = new JScrollPane(content);
        add(scrollPane, BorderLayout.CENTER);

        JToolBar toolBar = new JToolBar();

        JButton exportButton = new JButton("Export", UIUtils.getIconFromResources("save.png"));
        JPopupMenu exportMenu = UIUtils.addPopupMenuToComponent(exportButton);

//        JMenuItem saveRTF = new JMenuItem("as RTF (*.rtf)", UIUtils.getIconFromResources("filetype-text.png"));
//        saveRTF.addActionListener(e -> {
//            JFileChooser fileChooser = new JFileChooser();
//            fileChooser.setDialogTitle("Save as RTF");
//            if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
//                try {
//                    Files.write(fileChooser.getSelectedFile().toPath(), markdown.getBytes(Charsets.UTF_8));
//                } catch (IOException e1) {
//                    throw new RuntimeException(e1);
//                }
//            }
//        });
//        exportMenu.add(saveRTF);
//
//        JMenuItem savePDF = new JMenuItem("as PDF (*.pdf)", UIUtils.getIconFromResources("filetype-pdf.png"));
//        savePDF.addActionListener(e -> {
//            JFileChooser fileChooser = new JFileChooser();
//            fileChooser.setDialogTitle("Save as PDF");
//            if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
//                PdfConverterExtension.exportToPdf(fileChooser.getSelectedFile().toString(), toHTML(), "", OPTIONS);
//            }
//        });
//        exportMenu.add(savePDF);

        toolBar.add(exportButton);

//        JButton printButton = new JButton("Print", UIUtils.getIconFromResources("print.png"));
//        printButton.addActionListener(e -> {
//            try {
//                content.print();
//            } catch (PrinterException e1) {
//                throw new RuntimeException(e1);
//            }
//        });
//        toolBar.add(printButton);

        add(toolBar, BorderLayout.NORTH);
    }

    public void loadFrom(InputStream stream) throws IOException {
        try {
            rtfEditorKit.read(stream, content.getDocument(), 0);
        } catch (BadLocationException e) {
            throw new RuntimeException(e);
        }
        SwingUtilities.invokeLater(() -> scrollPane.getVerticalScrollBar().setValue(0));
    }

    public static RTFReader fromResource(String resourcePath) {
        RTFReader reader = new RTFReader();
        try {
            reader.loadFrom(ResourceUtils.getPluginResourceAsStream(resourcePath));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return reader;
    }
}
