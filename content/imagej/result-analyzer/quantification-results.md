+++
title = "Browsing quantification results"
weight = 20
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

<p >MISA++ applications store
quantification results in a standardized and flexible format that is
capable of organizing large amounts of metadata. The data is stored
as <u>objects</u> such as  quantities with units, locations or
application-specific objects such as glomeruli.
</p>
<p ><br/>

</p>
<p >MISA++ for ImageJ includes a tool to
filter, browse and process the quantification results and create
tables of only the relevant information.</p>
<p ><br/>

</p>
<p ><b>Important</b>: The tool is designed
to handle millions of data entries and uses a SQLite database to
handle such an amount of data. The database is created by the
<img src="/img/imagej/userguide_html_36868a0d47746c46.png" name="image20.png" class="inline-image"  border="0"/>
<i>MISA++
Result Analyzer</i> application. Please check if it is installed and
in the list of available applications (see <a href="#_3owr066okhh4"><font color="#1155cc"><u>Managing
applications</u></font></a>).</p>
<p ><img src="/img/imagej/userguide_html_cf0988573ecf97d5.png" name="image48.png"  border="0"/>
</p>
<p ><br/>

</p>
<p >The user interface is divided into
three sections:</p>
<ol>
	<li><p >An interface to filter the
	quantification results</p>
	<li><p >A tree to navigate the filtered
	results</p>
	<li><p >The selected object(s) displayed
	as table or object</p>
</ol>
<p ><br/>

</p>
<p >The data flows from left to right:
Quantification results are filtered by the filter tool and then
displayed in the object tree. You can browse the tree and further
refine your selection that will be displayed as table or object.</p>
<h3 class="western"><a name="_ibs3sprsjgc8"></a>Filtering</h3>
<p ><img src="/img/imagej/userguide_html_d818d1e834dab870.png" name="image8.png" class="inline-image"  border="0"/>
</p>
<p >To add a filter, click
<img src="/img/imagej/userguide_html_5d62cd2cd5f8a97b.png" name="image4.png" class="inline-image" border="0"/>
Add
filter and select one of the filter types. Each filter has a button
<img src="/img/imagej/userguide_html_5a1cd430de89e54f.png" name="image35.png" class="inline-image" border="0"/>
<i>Remove
filter</i> that deletes the filter from the list and a toggle
<img src="/img/imagej/userguide_html_efe62ab44d93c07c.png" name="image143.png" class="inline-image" border="0"/>
<i>Disable
filter</i> that temporarily disables the filter without deleting it.</p>
<p ><br/>

</p>
<p >Following filters are included in
MISA++ for ImageJ:</p>
<p ><br/>

</p>
<table cellpadding="7" cellspacing="0">
	<col />

	<col />

	<tr >
		<td ><p >
			<b>Filter</b></p>
		</td>
		<td ><p >
			<b>Description</b></p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_7e9251409421e5be.png" name="image95.png" class="inline-image" border="0"/>
<i>Filter
			by SQL</i></p>
		</td>
		<td ><p >
			Allows insertion of a custom SQL filter query.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_5104d2b86e357164.png" name="image103.png" class="inline-image" border="0"/>
<i>Filter
			by data</i></p>
		</td>
		<td ><p >
			Filters quantification results that are attached to specific data.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_def6d663c94b097e.png" name="image61.png" class="inline-image" border="0"/>
<i>Filter
			by object type</i></p>
		</td>
		<td ><p >
			Restricts the object types (e.g. only list glomeruli).</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_e507c96bf16d7da5.png" name="image99.png" class="inline-image" border="0"/>
<i>Filter
			by sample</i></p>
		</td>
		<td ><p >
			Only shows quantification results of specified samples.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_def6d663c94b097e.png" name="image57.png" class="inline-image" border="0"/>
<i>Filter
			only direct attachments</i></p>
		</td>
		<td ><p >
			MISA++ quantification results are hierarchical. With this filter,
			objects deeper down in the hierarchy are hidden.</p>
		</td>
	</tr>
</table>
<p ><br/>

</p>
<p ><br/>

</p>
<p ><b>Tip</b>: If you want to re-use the
filters for your own SQLite query, click
<img src="/img/imagej/userguide_html_18054d0fb7b17680.png" name="image43.png" class="inline-image" border="0"/>
<i>Copy
filters as SQL query</i> to obtain the query.</p>
<h3 class="western"><a name="_5716h12dkoke"></a>Object tree</h3>
<p ><img src="/img/imagej/userguide_html_57774bc496003f24.png" name="image9.png" class="inline-image" border="0"/>
</p>
<p >The object tree lists all unfiltered
objects in a hierarchy and acts as input for the table and object
browser.</p>
<p ><br/>

</p>
<p >The toolbar has following actions:</p>
<p ><br/>

</p>
<table cellpadding="7" cellspacing="0">
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
			<img src="/img/imagej/userguide_html_7e9251409421e5be.png" name="image2.png" class="inline-image" border="0"/>
<i>Automatically
			update</i></p>
		</td>
		<td ><p >
			If enabled (default), the tree is automatically updated when a
			filter is changed.</p>
			<p ><br/>

			</p>
			<p >Disable this feature if you
			change lots of filter settings at once and don’t want to wait
			for the database update.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_741a8f89f0bfae4f.png" name="image5.png" class="inline-image" border="0"/>
<i>Update</i></p>
		</td>
		<td ><p >
			Updates the tree manually.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_18054d0fb7b17680.png" name="image70.png" class="inline-image" border="0"/>
<i>Copy
			as SQL query</i></p>
		</td>
		<td ><p >
			Copies the current selection as SQL query. This includes the SQL
			query from filtering.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_6255d177ec99bfcf.png" name="image12.png" class="inline-image" border="0"/>
<i>Display
			by data</i></p>
		</td>
		<td ><p >
			If enabled (default), the tree is organized by sample → data →
			sub-data → type → property.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_def6d663c94b097e.png" name="image81.png" class="inline-image" border="0"/>
<i>Display
			by object type</i></p>
		</td>
		<td ><p >
			If enabled, the tree is organized by MISA++ application → type →
			data → sample → property.</p>
		</td>
	</tr>
</table>
<p ><br/>

</p>
<h4><a name="_58qdv7d84zkp"></a>Creating a table and browsing objects</h4>
<p >The data browser allows you to either
browse the quantification results as objects or create a table
(default option).</p>
<p ><br/>

</p>
<p ><img src="/img/imagej/userguide_html_eeecc2704778650f.png" name="image13.png" border="0"/>
</p>
<p >The table creator has following
actions:</p>
<p ><br/>

</p>
<p ><br/>

</p>
<table cellpadding="7" cellspacing="0">
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
			<img src="/img/imagej/userguide_html_7e9251409421e5be.png" name="Image6" class="inline-image" border="0"/>
<i>Automatically
			update</i></p>
		</td>
		<td ><p >
			If enabled (default), the table is automatically updated when a
			filter is changed or the selection the tree changes.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_741a8f89f0bfae4f.png" name="Image7" class="inline-image" border="0"/>
<i>Update</i></p>
		</td>
		<td ><p >
			Updates the table manually.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_767000c5d105b06a.png" name="image88.png" class="inline-image" border="0"/>
Export</p>
		</td>
		<td ><p >
			Saves the table as *.csv or *.xlsx</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_f8c87f207b233901.png" name="image7.png" class="inline-image" border="0"/>
Analyze</p>
		</td>
		<td ><p >
			Opens a tool to further analyze the table (see <a href="#_qyp5a485ja6p"><font color="#1155cc"><u>Summarizing
			quantification results</u></font></a>).</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_def6d663c94b097e.png" name="image86.png" class="inline-image" border="0"/>
Current
			object</p>
		</td>
		<td ><p >
			Tables can only be created for one object type. Use this selection
			to change the object type.</p>
		</td>
	</tr>
	<tr >
		<td ><p >
			<img src="/img/imagej/userguide_html_60dcc39329925810.png" name="image115.png" class="inline-image" border="0"/>
Edit
			columns</p>
		</td>
		<td ><p >
			By default, the table does not contain all properties of the
			current object. The column editor allows you to include more
			columns or exclude unnecessary information.</p>
		</td>
	</tr>
</table>
<p ><br/>

</p>
<p >By changing the current mode from
<img src="/img/imagej/userguide_html_e647de0bbe7b13bb.png" name="image31.png" class="inline-image" border="0"/>
<i>Table</i>
to
<img src="/img/imagej/userguide_html_def6d663c94b097e.png" name="image123.png" class="inline-image" border="0"/>
<i>Object
list</i>, all selected quantification data is displayed as objects.
You can browse the list and export objects in JSON format.</p>
