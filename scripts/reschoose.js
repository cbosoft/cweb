function simpleresratio(r1, r2) {
  return r1 / r2;
}

function vdivresratio(r1, r2) {
  return r2/(r1+r2);
}

function noninvrestatio(r1, r2) {
  return (1+(r2/r1));
}

function invrestatio(r1, r2) {
  return r2/r1;
}




function get_situation() {
  if (document.getElementById("situationvdiv").checked) {
    return 1;
  }
  else if (document.getElementById("situationinv").checked) {
    return 2;
  }
  else if (document.getElementById("situationnoninv").checked) {
    return 3;
  }
  else {
    return 0;
  }
}




function reschoose() {
  var resistors = document.getElementById("resistorlist").value.split(",");
  var ratio = document.getElementById("ratio").value;
  var numberresults = document.getElementById("number").value;

  if (ratio < 0.0) ratio *= -1.0;

  var situation = get_situation();
  var getratio = simpleresratio;

  switch (situation) {
    case 1:
      console.log("VDIV");
      getratio = vdivresratio;
      break;
    case 2:
      getratio = invrestatio;
      break;
    case 3:
      getratio = noninvrestatio;
      break;
  }
  
  var i, j;
  var lb, ub;
  lb = ratio * 0.9;
  ub = ratio * 1.1;

  var valid = [];
  var score = [];

  for (i = 0; i < resistors.length; i++) {
    for (j = i+1; j < resistors.length; j++) {
      var rat, ratinv, r1 = Number(resistors[i]), r2 = Number(resistors[j]);
      console.log(r1);
      rat = getratio(r1, r2);
      ratinv = getratio(r2, r1);

      if (rat > lb && rat < ub) {

        console.log(ratio, rat, r1, r2, "rat");
        valid.push([resistors[i], resistors[j]]);
        score.push(((rat-ratio) ** 2.0) ** 0.5);
      }
      else if (ratinv > lb && ratinv < ub) {

        console.log(ratio, ratinv, r1, r2, "inv");
        valid.push([resistors[j], resistors[i]]);
        score.push(((ratinv-ratio) ** 2.0) ** 0.5);
      }

    }
  }

  if (valid.length == 0) {
    document.getElementById("results").innerHTML = "<b>No results found.</b>";
    return;
  }

  var best = [];
  var bestscore = [];
  for (i = 0; i < Math.min(numberresults, valid.length); i++) {
    var idx = score.indexOf(Math.min(...score));
    bestscore.push(score.splice(idx, 1));
    best.push(valid.splice(idx, 1));
  }


  var resstr;
  resstr  = "<h3>Results</h3>"
  resstr += "<table class=\"centre\">";
  resstr += "<tr><th>\\(R_1(\\Omega)\\)</th><th>\\(R_2(\\Omega)\\)</th><th>Ratio</th></tr>";
  for (i = 0; i < best.length; i++) {
    var pair = best[i][0];
    var scorestr = bestscore[i][0].toFixed(4);
    var larg = Math.max(...pair);
    var smol = Math.min(...pair);
    var r1, r2, ratio;

    if (ratio > 1.0) {
      r1 = smol;
      r2 = larg;
    }
    else {
      r1 = larg;
      r2 = smol;
    }

    ratio = getratio(r1, r2).toFixed(3);

    resstr += `<tr><td>${r1}</td><td>${r2}</td><td>${ratio}</td></tr>`
  }
  resstr += "</table>"
  document.getElementById("results").innerHTML = resstr;

  // make everything math-y
  MathJax.Hub.Queue(["Typeset", MathJax.Hub]);

}

function update_ratio_label(){
  var situation = get_situation();

  var expl= document.getElementById("ratioexpl");

  switch (situation) {
    case 1:
      expl.innerHTML  = "For a voltage divider:";
      expl.innerHTML += "$$\\frac{V_{out}}{V_{in}} = \\frac{R_2}{R_1 + R_2}$$";
      expl.innerHTML += "Enter the <b>voltage ratio</b> below.";
      break;
    case 2:
      expl.innerHTML  = "For an inverting op-amp:<br><img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/4/41/Op-Amp_Inverting_Amplifier.svg/300px-Op-Amp_Inverting_Amplifier.svg.png\" class=\"centre\"/>";
      expl.innerHTML += "$$\\frac{V_{out}}{V_{in}} = -\\frac{R_f}{R_{in}} = -\\frac{R_2}{R_1}$$";
      expl.innerHTML += "Enter the <b>magnitude</b> of the desired <b>voltage ratio</b> below.";
      break;
    case 3:
      expl.innerHTML  = "For an inverting op-amp:<br><img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/4/44/Op-Amp_Non-Inverting_Amplifier.svg/300px-Op-Amp_Non-Inverting_Amplifier.svg.png\" class=\"centre\"/>";
      expl.innerHTML += "$$\\frac{V_{out}}{V_{in}} = \\left(1 + \\frac{R_2}{R_{1}}\\right)$$";
      expl.innerHTML += "Enter the desired <b>voltage ratio</b> below.";
      break;
    case 0:
      expl.innerHTML  = "For any ratio of resistances given by:";
      expl.innerHTML += "$$\\frac{R_1}{R_2}$$";
      expl.innerHTML += "Enter the desired <b>resistor ratio</b> below."
      break;
  }
  MathJax.Hub.Queue(["Typeset", MathJax.Hub]);
}
