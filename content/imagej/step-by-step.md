+++
title = "Tutorial: Step by step analysis"
weight = 20
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

<p>The following step-by-step tutorial shows the quantitative analysis of glomeruli by creating statistics and plots of their diameter.</p>

<table cellpadding="7" cellspacing="0">
	<col />

	<col />

	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_cb89aec853a553cd.png" name="image138.png" border="0"/>
</p>
		</td>
		<td ><p >
			Start ImageJ and open the MISA++ plugin by navigating to <i>Plugins
			→ MISA++ for ImageJ.</i> This will open a list of all available
			MISA++ applications.
			</p>
			<p >Select “<img class="inline-image" src="/img/imagej/userguide_html_36868a0d47746c46.png" name="image15.png" border="0"/>
<i>MISA++
			Kidney Glomeruli Segmentation</i>”.</p>
			<p ><br/>

			</p>
			<p >Click
			<img src="/img/imagej/userguide_html_63631d3be680189.png" class="inline-image" name="image80.png" border="0"/>
<i>Launch</i>
			to open a tool that allows preparation of the data and changing
			parameters.</p>
			<p ><br/>

			</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p>
			<img src="/img/imagej/userguide_html_d51c71947cefea0d.png" name="image128.png" border="0"/>
</p>
		</td>
		<td ><p >
			Click
			<img src="/img/imagej/userguide_html_b477416cd79efea5.png" class="inline-image" name="image10.png" border="0"/>
<i>Import
			folder</i> and select the folder that contains the example data
			set to automatically add samples and import images.</p>
			<p ><br/>

			</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p>
			<img src="/img/imagej/userguide_html_ffb1140b27a42629.png" name="image16.png" border="0"/>
</p>
		</td>
		<td ><p >
			Select the folder that contains our example data set and click
			<i>Open</i>.</p>
			<p ><br/>

			</p>
			<p>Then delete the default sample “<img class="inline-image" src="/img/imagej/userguide_html_e507c96bf16d7da5.png" name="image141.png" border="0"/>
<i>New
			Sample</i>”, as we already imported all necessary data.</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p>
			<img src="/img/imagej/userguide_html_4d9a02d8fa94c953.png" name="image125.png" border="0"/>
</p>
		</td>
		<td ><p >
			To enable multi-threading, select the
			<img src="/img/imagej/userguide_html_7e9251409421e5be.png" class="inline-image" name="image109.png" border="0"/>
<i>Runtime</i>
			tab and increase the number of threads.</p>
			<p ><br/>

			</p>
			<p >Click
			<img src="/img/imagej/userguide_html_63631d3be680189.png" class="inline-image" name="image76.png" border="0"/>
<i>Run</i>
			and then <i>Run now</i> to start the analysis. After the analysis
			finished, the plugin will ask you to further analyze the results.</p>
			<p ><br/>

			</p>
		</td>
	</tr>
</table>
<h3 class="western"><a name="_bmwm3pe7xyb6"></a><br/>
<br/>

</h3>
<h1>Example: Analyzing results</h1>
<p ><br/>

</p>
<table cellpadding="7" cellspacing="0">
	<col />

	<col />

	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_7e1d387f696b79e7.png" name="image33.png" border="0"/>
</p>
		</td>
		<td ><p >
			The analysis tool displays a list of all output data and (if
			available) input data and allows importing data back into ImageJ.</p>
			<p >If you want to import the
			glomeruli into ImageJ, click
			<img src="/img/imagej/userguide_html_19ddc6399d334fb2.png" class="inline-image" name="image19.png" border="0"/>
<i>Bioformats
			Import</i> next to the “<i>glomeruli3d</i>” data.</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_1f3554799e860fe8.png" name="image105.png" border="0"/>
</p>
		</td>
		<td ><p >
			To analyze quantification results, click
			<img src="/img/imagej/userguide_html_f8c87f207b233901.png" class="inline-image" name="image98.png" border="0"/>
<i>Browse
			quantification results</i>.</p>
			<p ><br/>

			</p>
			<p >This will open a tool to browse
			and filter quantification results and transform them into tables
			that can be plotted.</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_7c7db2265df6ffa2.png" name="image145.png" border="0"/>
</p>
		</td>
		<td ><p >
			To analyze the glomeruli, select
			<img src="/img/imagej/userguide_html_def6d663c94b097e.png" class="inline-image" name="image68.png" border="0"/>
<i>Objects</i>
			in the middle column and then “<img src="/img/imagej/userguide_html_def6d663c94b097e.png" class="inline-image" name="image127.png" border="0"/>
<i>Glomerulus</i>”
			above the table.</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/column_editor.png" name="image139.png" border="0"/>
</p>
		</td>
		<td ><p >
			Due to performance reasons, the table does not contain all
			available information about each glomerulus. To add more, click
						<img src="/img/imagej/userguide_html_60dcc39329925810.png" class="inline-image" name="image92.png" border="0"/>
<i>Edit
			columns</i>.</p>
			<p ><br/>

			</p>
			<p >Click
			<img src="/img/imagej/userguide_html_96edca7cbbbe705d.png" class="inline-image" name="image118.png" border="0"/>
<i>Clear
			selection</i> and select the columns
			</p>
			<ul>
				<li><p >Sample</p>
				<li><p >diameter/Value</p>
				<li><p >diameter/Unit/unit</p>
				<li><p >Valid</p>
			</ul>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/misaxx_analyzer_table.png" align="bottom"/>
</p>
		</td>
		<td ><p >
			If you are happy with the table, click
			<img src="/img/imagej/userguide_html_f8c87f207b233901.png" class="inline-image" name="image85.png" border="0"/>
<i>Analyze</i>.
						</p>
			<p ><br/>

			</p>
			<p >This will open a tool that allows
			you to modify the table and perform statistics.</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_6c577a41ad99985a.png" name="image140.png" border="0"/>
</p>
		</td>
		<td ><p >
			To calculate mean and variance of the diameter,
			</p>
			<p >select a cell
			in the “diameter/value” column and navigate to
			<img src="/img/imagej/userguide_html_cc4902d6f1dbab6a.png" class="inline-image" name="image32.png" border="0"/>
<i>Add
			column → </i>
			<img src="/img/imagej/userguide_html_18054d0fb7b17680.png" class="inline-image" name="image79.png" border="0"/>
<i>Copy
			selected column.</i> Then click “OK”.</p>
			<p ><br/>

			</p>
			<p ><br/>

			</p>
			<p ><br/>

			</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_45fc33e19f6e192d.png" name="image136.png" border="0"/>
</p>
		</td>
		<td ><p >
			Click “<i>Integrate columns</i>” at the top right corner and
			assign following roles to each column:</p>
			<ul>
				<li><p >Sample:
				<b>Category</b></p>
				<li><p >diameter/unit/unit:
				<b>Ignore</b></p>
				<li><p >diameter/value:
				<b>Average</b></p>
				<li><p >valid:
				<b>Category</b></p>
				<li><p >diameter/value:
				<b>Variance</b></p>
			</ul>
			<p >Click “Calculate”.</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_ef336f90674ef339.png" name="image93.png" border="0"/>
</p>
		</td>
		<td ><p >
			The table now shows the mean and variance of the glomeruli,
			categorized by the sample and if the glomerulus is valid according
			to the segmentation algorithm.</p>
			<p ><br/>

			</p>
			<p >Click
			<img src="/img/imagej/userguide_html_a590cddcece250fa.png" class="inline-image" name="image24.png" border="0"/>
<i>Undo</i>
			to restore the original table and then
			<img src="/img/imagej/userguide_html_f8c87f207b233901.png" class="inline-image" name="image45.png" border="0"/>
<i>Create
			plot</i>.</p>
		</td>
	</tr>
	<tr valign="top">
		<td ><p >
			<img src="/img/imagej/userguide_html_67c0eb0b5ecaaf0d.png" name="image117.png" border="0"/>
</p>
		</td>
		<td ><p >
			Set the plot type to
			<img src="/img/imagej/userguide_html_533bfc54d0d17f9f.png" class="inline-image" name="image110.png" border="0"/>
<i>Box
			Plot</i>, set the category to “<i>Table.Sample</i>”, X axis to
			“<i>Table.valid</i>” and the list of values to the diameter.</p>
		</td>
	</tr>
</table>
