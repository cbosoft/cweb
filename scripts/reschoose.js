function reschoose() {
  var resistors = document.getElementById("resistorlist").value.split(",");
  var ratio = document.getElementById("ratio").value;
  var numberresults = document.getElementById("number").value;
  
  var i, j;
  var lb, ub;
  lb = ratio * 0.9;
  ub = ratio * 1.1;

  var valid = [];
  var score = [];

  for (i = 0; i < resistors.length; i++) {
    for (j = i+1; j < resistors.length; j++) {
      var rat, ratinv;
      rat = resistors[i]/resistors[j];
      ratinv = 1.0/rat;

      var pair = [resistors[i], resistors[j]];

      if ((rat > lb && rat < ub) || (ratinv > lb && ratinv < ub)) {
        valid.push(pair);
        
        var score1 = ((rat-ratio) ** 2.0) ** 0.5;
        var score2 = ((ratinv-ratio) ** 2.0) ** 0.5;
        score.push(Math.min(score1, score2));
      }
    }
  }

  if (valid.length == 0) {
    document.getElementById("results").innerHTML = "<b>No results found.</b>";
  }

  var best = [];
  var bestscore = [];
  for (i = 0; i < Math.min(numberresults, valid.length); i++) {
    var idx = score.indexOf(Math.min(...score));
    bestscore.push(score.splice(idx, 1));
    best.push(valid.splice(idx, 1));
  }

  var resstr = "<h3>Results</h3><ul>";
  for (i = 0; i < best.length; i++) {
    var pair = best[i][0];
    var scorestr = bestscore[i][0].toFixed(4);
    var larg = Math.max(...pair);
    var smol = Math.min(...pair);
    var r1, r2;

    if (ratio > 1.0) {
      r1 = smol;
      r2 = larg;
    }
    else {
      r1 = larg;
      r2 = smol;
    }

    resstr += `<li>\\(R_1 = ${r1}\\,\\Omega\\), \\(R_2 = ${r2}\\,\\Omega\\)</li>`
  }
  resstr += "</ul>"
  document.getElementById("results").innerHTML = resstr;
  MathJax.Hub.Queue(["Typeset", MathJax.Hub]);

}
