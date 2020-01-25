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

package org.hkijena.misa_imagej;

import io.scif.services.DatasetIOService;
import net.imagej.DatasetService;
import net.imagej.ImageJ;
import net.imagej.ops.OpService;
import org.hkijena.misa_imagej.ui.repository.MISAModuleRepositoryUI;
import org.scijava.app.StatusService;
import org.scijava.command.Command;
import org.scijava.command.CommandService;
import org.scijava.display.DisplayService;
import org.scijava.log.LogService;
import org.scijava.plugin.Parameter;
import org.scijava.plugin.Plugin;
import org.scijava.plugin.PluginService;
import org.scijava.thread.ThreadService;
import org.scijava.ui.UIService;

import javax.swing.*;

@Plugin(type = Command.class, menuPath = "Plugins>MISA++ for ImageJ ...")
public class MISAImageJCommand implements Command {

	@Parameter
	private OpService ops;

	@Parameter
	private LogService log;

	@Parameter
	private UIService ui;

	@Parameter
	private CommandService cmd;

	@Parameter
	private StatusService status;

	@Parameter
	private ThreadService thread;

	@Parameter
	private DatasetIOService datasetIO;

	@Parameter
	private DisplayService display;

	@Parameter
	private DatasetService datasetService;

	@Parameter
	private PluginService pluginService;

	/**
	 * show a dialog and give the dialog access to required IJ2 Services
	 */
	@Override
	public void run() {
	    MISAImageJRegistryService.instantiate(pluginService);
		SwingUtilities.invokeLater(() -> {
			ToolTipManager.sharedInstance().setDismissDelay(Integer.MAX_VALUE);
			ToolTipManager.sharedInstance().setInitialDelay(1000);
			MISAModuleRepositoryUI.getInstance(this).setVisible(true);
		});
	}

	public LogService getLogService() {
		return log;
	}

	public StatusService getStatusService() {
		return status;
	}

	public ThreadService getThreadService() {
		return thread;
	}

	public UIService getUiService() {
		return ui;
	}

	public DatasetIOService getDatasetIOService() {
		return datasetIO;
	}

	public DisplayService getDisplayService() {
		return display;
	}

	public DatasetService getDatasetService() {
		return datasetService;
	}

	public static void main(final String... args) {
		final ImageJ ij = new ImageJ();
		ij.ui().showUI();
		ij.command().run(MISAImageJCommand.class, true);
	}

	public PluginService getPluginService() {
		return pluginService;
	}
}

