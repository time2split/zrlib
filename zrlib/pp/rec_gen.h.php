<?php
$CONFIG =
[
"nargs.max" => 128,
"num.max" => 1024
];
?>
#ifndef ZRPP_REC_H
#error "No manual include allowed"
#endif

#define ZRINC(x) ZRCAT2(ZRINC_, x)
<?php
for($i = 0; $i < $CONFIG['num.max']; $i++)echo "#define ZRINC_$i ",$i + 1,"\n";
?>

#define ZRDEC(x) ZRCAT2(ZRDEC_, x)
#define ZRDEC_0 0
<?php
for($i = 1; $i <= $CONFIG['num.max']; $i++)echo "#define ZRDEC_$i ",$i - 1,"\n";
?>

#define ZRDEFERN(N) ZRCAT2(ZRDEFER,N)()
<?php
for($i = 0; $i <= $CONFIG['nargs.max']; $i++)
{
	echo "#define ZRDEFER$i() ";

	for($j = 0; $j < $i; $j++)echo "ZREMPTY ";
	for($j = 0; $j < $i; $j++)echo "()";
	echo "\n";
}?>

#define ZRNARGS_MAX <?=$CONFIG['nargs.max']?>

#define _ZRNARGS_GETN(<?php
for($i = 0; $i < $CONFIG['nargs.max']; $i++)echo "_$i,"
?>N,...) N
#define _ZRNARGS_REVERSE <?php
for($i = $CONFIG['nargs.max']; $i >= 0; $i--)echo "$i", $i != 0 ? "," : "";
echo "\n";
?>

/* https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/ */
#define ZRARGS_HAS_COMMA(...) _ZRNARGS_GETN(__VA_ARGS__, <?= implode(',', array_fill(0, $CONFIG['nargs.max'] - 1, 1))?>,0)
