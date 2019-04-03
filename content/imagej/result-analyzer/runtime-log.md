+++
title = "Analyzing the runtime"
weight = 50
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

<p >MISA++ applications always track basic
information about the time needed to do an analysis with the option
to enable a detailed runtime log. It can be accessed via the analysis
tool (see <a href="#_a35e5oxzxp03"><font color="#1155cc"><u>Evaluating
output data</u></font></a>) or in a standalone-version that is
accessible via the list of installed MISA++ applications (see
<a href="#_3owr066okhh4"><font color="#1155cc"><u>Managing
applications</u></font></a>).</p>
<p ><img src="/img/imagej/userguide_html_91b8741ccde0c481.png" name="image104.png" align="bottom"   border="0"/>
</p>
<p >The timeline is a Gantt-chart that
lists the duration and thread allocation of each calculation step. A
full-detailed version that lists each individual task (e.g.  tissue
segmentation) is only available if enabled in the MISA++ application
parameters.</p>
<p ><br/>

</p>
<p >The tool also creates statistics such
as the total runtime, estimated single-threaded runtime and the
estimated multithreading speedup.</p>
