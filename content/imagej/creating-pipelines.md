+++
title = "Creating pipelines"
weight = 50
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

<p >MISA++ applications can make usage of
other MISA++ applications via a fixed code dependency. Creation of
pipelines using code dependencies on the other hand requires
modification of the source code. The MISA++ ImageJ plugin provides a
tool that allows creation of pipelines of existing MISA++
applications without writing code.</p>
<p ><img src="/img/imagej/userguide_html_f4119b090e97ecaa.png" name="image134.png"    border="0"/>
</p>
<p >The user interface is divided into four
sections:</p>
<ol>
	<li><p >Pipeline flow chart</p>
	<li><p >List of available MISA++
	applications (modules) and an overview of samples</p>
	<li><p >Toolbar with global actions</p>
	<li><p >Parameter validation results (for
	all pipeline nodes)</p>
</ol>
<h2 class="western"><a name="_1mf73memxs8o"></a>Pipeline flow chart</h2>
<p >The pipeline flow chart ( represents
MISA++ applications as processing steps and data flow as arrow
(connections) between processing steps.</p>
<p ><img src="/img/imagej/userguide_html_dcd645e01b36f568.gif" />
</p>
<p >Each processing step consists of
following components (from top to bottom):</p>
<p ><br/>

</p>
<table  cellpadding="7" cellspacing="0">
	<col />

	<col />

	<tr >
		<td ><p >
			<b>Component</b></p>
		</td>
		<td ><p >
			<b>Description</b></p>
		</td>
	</tr>
	<tr >
		<td ><p >
			Name</p>
		</td>
		<td ><p >
			The name of the processing step.</p>
			<p >Can be edited.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			Description</p>
		</td>
		<td ><p >
			Optional description of the processing step. Can be edited.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_339b3b21fe54de67.png" name="image26.png" class="inline-image" border="0"/>
<i>Connect
			from other node</i></p>
		</td>
		<td ><p >
			Click to connect another processing step to the current one.</p>
			<p >This button is not visible if
			there are no available connections.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_c986631bbf8e412c.png" name="image77.png" class="inline-image" border="0"/>
<i>Remove
			entry</i></p>
		</td>
		<td ><p >
			Removes the processing step.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_60dcc39329925810.png" name="image23.png" class="inline-image" border="0"/>
<i>Edit
			parameters</i></p>
		</td>
		<td ><p >
			Opens a parameter editor (see <a href="#_cwa392wuuuay"><font color="#1155cc"><u>Analyzing
			data</u></font></a>) for the MISA++ application behind the
			processing step.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			Arrow(s) and
			<img src="/img/imagej/userguide_html_c986631bbf8e412c.png" name="image112.png" class="inline-image" border="0"/>
<i>Remove
			connection</i></p>
		</td>
		<td ><p >
			An arrow connects the data from one application to another. Click
						<img src="/img/imagej/userguide_html_c986631bbf8e412c.png" name="Image1" class="inline-image" border="0"/>
<i>Remove
			connection </i>to remove the connection.</p>
		</td>
	</tr>
</table>
<p ><br/>

</p>
<h2 class="western"><a name="_b41sgyvahxl1"></a>Managing samples</h2>
<p >By default, all MISA++ applications
within the pipeline have the same set of samples. You can disable
this behavior by navigating the “<i>Samples</i>” tab next to the
pipeline and disabling
<img src="/img/imagej/userguide_html_339b3b21fe54de67.png" name="image38.png" class="inline-image" border="0"/>
<i>Autosync</i>.
The interface  contains a list of all
<img src="/img/imagej/userguide_html_e507c96bf16d7da5.png" name="image102.png" class="inline-image" border="0"/>
samples,
color-coded by the MISA++ applications that work on the sample.</p>
<p >Below the list, you can find following
actions:</p>
<p ><br/>

</p>
<table  cellpadding="7" cellspacing="0">
	<col />

	<col />

	<tr >
		<td ><p >
			<b>Action</b></p>
		</td>
		<td ><p >
			<b>Description</b></p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_339b3b21fe54de67.png" name="Image2" class="inline-image" border="0"/>
<i>Synchronize
			selected</i></p>
		</td>
		<td ><p >
			Ensures that the selected samples are represented in the same set
			of processing steps.</p>
			<p ><br/>

			</p>
			<p ><u>Example</u></p>
			<p >We have
			following configuration:</p>
			<p ><br/>

			</p>
			<table  cellpadding="7" cellspacing="0">
				<col />

				<col />

				<col />

				<col />

				<tr >
					<td ><p >
						<br/>

						</p>
					</td>
					<td ><p >
						Sample1</p>
					</td>
					<td ><p >
						Sample2</p>
					</td>
					<td ><p >
						Sample3</p>
					</td>
				</tr>
				<tr >
					<td ><p >
						Step 1</p>
					</td>
					<td ><p >
						<br/>

						</p>
					</td>
					<td ><p >
						✓</p>
					</td>
					<td ><p >
						<br/>

						</p>
					</td>
				</tr>
				<tr >
					<td ><p >
						Step 2</p>
					</td>
					<td ><p >
						✓</p>
					</td>
					<td ><p >
						✓</p>
					</td>
					<td ><p >
						<br/>

						</p>
					</td>
				</tr>
				<tr >
					<td ><p >
						Step 3</p>
					</td>
					<td ><p >
						<br/>

						</p>
					</td>
					<td ><p >
						✓</p>
					</td>
					<td ><p >
						✓</p>
					</td>
				</tr>
			</table>
			<p >
			<br/>

			</p>
			<p >
			If we synchronize Sample 1 and Sample 3, both of them will be in
			Step 2 and Step 3, but not Step 1.
			</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_339b3b21fe54de67.png" name="Image3" class="inline-image"  border="0"/>
<i>Autosync</i></p>
		</td>
		<td ><p >
			If enabled (default), keeps samples synchronized across all
			processing steps.</p>
		</td>
	</tr>
</table>
<h2 class="western"><a name="_d80m0c4l0gbs"></a>Creating a pipeline</h2>
<p >To add an application to the pipeline,
select an application and click
<img src="/img/imagej/userguide_html_5d62cd2cd5f8a97b.png" name="image82.png" class="inline-image" border="0"/>
<i>Add
to pipeline</i>. This will create a new processing step in the flow
chart. You can use your mouse to drag the processing step to any
location in the flow chart.</p>
<p ><br/>

</p>
<p >To implement the flow of data from one
application to another, a connection must be created. Click the
<img src="/img/imagej/userguide_html_339b3b21fe54de67.png" name="Image4" class="inline-image" border="0"/>
<i>Connect
from other node </i>button on the <u>target</u> processing step and
select the <u>source</u> processing step. This will create an arrow
and will allow you to import data from another processing step.</p>
<h2 class="western"><a name="_1fosiqtvikya"></a>Connecting data</h2>
<p >Creating a connection between
processing steps does not automatically connect the output of the
source to the input of the target processing step.
</p>
<p >To connect data, open the parameter
editor of the <u>target</u> processing step via
<img src="/img/imagej/userguide_html_60dcc39329925810.png" name="Image5" class="inline-image" border="0"/>
<i>Edit
parameters </i>and change the importer (see <a href="#_694zqz3sztxg"><font color="#1155cc"><u>Importers</u></font></a>)
of the input data to
<img src="/img/imagej/userguide_html_5104d2b86e357164.png" name="image66.png" class="inline-image" border="0"/>
<i>Pipeline:
&lt;Name of the source processing step&gt;. </i>Then select the
appropriate data from the available options.</p>
<p ><img src="/img/imagej/userguide_html_bbea8f39791f1d16.gif" />
</p>
<h2 class="western"><a name="_rex69vs233a1"></a>Pipeline actions</h2>
<p >Following actions are available at in
the toolbar:</p>
<p ><br/>

</p>
<table  cellpadding="7" cellspacing="0">
	<col />

	<col />

	<tr >
		<td ><p >
			Action</p>
		</td>
		<td ><p >
			Description</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_b477416cd79efea5.png" name="image113.png" class="inline-image" border="0"/>
<i>Open</i></p>
		</td>
		<td ><p >
			Opens a pipeline description file.</p>
			<p >Please note that while structure
			of the pipeline and its connections are imported, all non-pipeline
			input data (from outside sources such as ImageJ) must be manually
			set after loading the pipeline.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_767000c5d105b06a.png" name="image55.png" class="inline-image" border="0"/>
<i>Save</i></p>
		</td>
		<td ><p >
			Saves the structure of the pipeline, including</p>
			<ul>
				<li><p >The
				processing steps</p>
				<li><p >Samples</p>
				<li><p >Algorithm
				parameters</p>
				<li><p >Sample
				parameters</p>
				<li><p >Runtime
				parameters</p>
				<li><p >Pipeline
				connections (including importer settings)</p>
			</ul>
			<p >This will not save importer
			settings for non-pipeline data sources.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_4d6d742dee2c4ba6.png" name="image40.png" class="inline-image" border="0"/>
<i>Check
			parameters</i></p>
		</td>
		<td ><p >
			Manually triggers a check if the settings of each processing step
			are correct. See <a href="#_nuktimj95nof"><font color="#1155cc"><u>Validating
			the current pipeline settings</u></font></a> for more information.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_ff801f752ab61775.png" name="image83.png" class="inline-image" border="0"/>
<i>Export</i></p>
		</td>
		<td ><p >
			Exports a ready-to-use package that processes the pipeline. The
			packages require that the MISA++ applications are installed on the
			current computer and includes all settings, parameters and data.</p>
			<p ><br/>

			</p>
			<p >The tool generates two
			feature-identical scripts <i>run.sh</i> (Linux) and <i>run.py</i>
			(any operating system) and saves the pipeline structure in<i>
			pipeline.json</i>.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_63631d3be680189.png" name="image44.png" class="inline-image" border="0"/>
<i>Run</i></p>
		</td>
		<td ><p >
			Executes the pipeline on the current computer.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_ef081c2083a666e2.png" name="image130.png" class="inline-image" border="0"/>
<i>Help</i></p>
		</td>
		<td ><p >
			Opens the documentation.</p>
		</td>
	</tr>
</table>
<p ><br/>

</p>
<h2 class="western"><a name="_nuktimj95nof"></a>Validating the
current pipeline settings</h2>
<p >Similar to the analysis with one
application (see <a href="#_me14otj4lyhl"><font color="#1155cc"><u>Analyzing
results</u></font></a>), the pipeline builder will validate if the
processing steps have valid parameters. See <a href="#_qyot2s350mgf"><font color="#1155cc"><u>Validating
the current settings</u></font></a> for more information.</p>
