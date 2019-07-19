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




function get_resistor_set_choice() {
  if (document.getElementById("E24").checked) {
    return 24;
  }
  else if (document.getElementById("E12").checked) {
    return 12;
  }
  else if (document.getElementById("E6").checked) {
    return 6;
  }
  else {
    return 0;
  }
}

function clear_results() {
  var e = document.getElementById("results");
  e.innerHTML = "";
}





function get_E_set(n, number_decades, start_decade) {
  
  if (n == 0)
    return [];

  var rv = [];
  var i, j;
  var mult = Math.pow(10, 1/Number(n));
  var ndecades = (number_decades == undefined) ? 6 : number_decades;
  var idecade = (start_decade == undefined) ? 0 : start_decade;


  var E192 = [1.00, 1.01, 1.02, 1.04, 1.05, 1.06, 1.07, 1.09, 1.10, 1.11, 1.13, 1.14, 1.15, 1.17, 1.18, 1.20, 1.21, 1.23, 1.24, 1.26, 1.27, 1.29, 1.30, 1.32, 1.33, 1.35, 1.37, 1.38, 1.40, 1.42, 1.43, 1.45, 1.47, 1.49, 1.50, 1.52, 1.54, 1.56, 1.58, 1.60, 1.62, 1.64, 1.65, 1.67, 1.69, 1.72, 1.74, 1.76, 1.78, 1.80, 1.82, 1.84, 1.87, 1.89, 1.91, 1.93, 1.96, 1.98, 2.00, 2.03, 2.05, 2.08, 2.10, 2.13, 2.15, 2.18, 2.21, 2.23, 2.26, 2.29, 2.32, 2.34, 2.37, 2.40, 2.43, 2.46, 2.49, 2.52, 2.55, 2.58, 2.61, 2.64, 2.67, 2.71, 2.74, 2.77, 2.80, 2.84, 2.87, 2.91, 2.94, 2.98, 3.01, 3.05, 3.09, 3.12, 3.16, 3.20, 3.24, 3.28, 3.32, 3.36, 3.40, 3.44, 3.48, 3.52, 3.57, 3.61, 3.65, 3.70, 3.74, 3.79, 3.83, 3.88, 3.92, 3.97, 4.02, 4.07, 4.12, 4.17, 4.22, 4.27, 4.32, 4.37, 4.42, 4.48, 4.53, 4.59, 4.64, 4.70, 4.75, 4.81, 4.87, 4.93, 4.99, 5.05, 5.11, 5.17, 5.23, 5.30, 5.36, 5.42, 5.49, 5.56, 5.62, 5.69, 5.76, 5.83, 5.90, 5.97, 6.04, 6.12, 6.19, 6.26, 6.34, 6.42, 6.49, 6.57, 6.65, 6.73, 6.81, 6.90, 6.98, 7.06, 7.15, 7.23, 7.32, 7.41, 7.50, 7.59, 7.68, 7.77, 7.87, 7.96, 8.06, 8.16, 8.25, 8.35, 8.45, 8.56, 8.66, 8.76, 8.87, 8.98, 9.09, 9.20, 9.31, 9.42, 9.53, 9.65, 9.76, 9.88];
  
  if (192 % n != 0) {
    alert("incorrect E series selected");
    return [];
  }

  var skip = 192/n;
  var p = (n < 48) ? 2 : 3;
  for (i = 0; i < 192; i += skip) {
    rv.push(Math.pow(10, idecade)*Number(E192[i].toPrecision(p)));
  }

  for (i = 1; i < ndecades; i++) {
    mult = Math.pow(10, i);
    for (j = 0; j < n; j++) {
      var res = mult*rv[j] 
      rv.push(Number(res.toPrecision(p)));
    }
  }
  return rv;
}




function update_res_ex() {
  var n = get_resistor_set_choice();
  var ex = document.getElementById("resex");
  ex.innerHTML = `<b>E${n}:</b> ` + get_E_set(n, 1).join(", ");
}





function get_resistor_set() {
  var n = get_resistor_set_choice();

  var rv = [];

  rv.push(...get_E_set(n));

  var extravalstr = document.getElementById("extra").value;

  if (extravalstr.length != 0) {
    var extrasplit = extravalstr.split(",");
    var i;
    for (i = 0; i < extrasplit.length; i++) {
      rv.push(Number(extrasplit[i]));
    }
  }

  return rv;
}




function reschoose() {
  var resistors = get_resistor_set();
  var ratio = document.getElementById("ratio").value;
  var numberresults = document.getElementById("number").value;



  if (ratio < 0.0) ratio *= -1.0;

  var situation = get_situation();
  var getratio = simpleresratio;

  switch (situation) {
    case 1:
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
      rat = getratio(r1, r2);
      ratinv = getratio(r2, r1);

      if (rat > lb && rat < ub) {

        valid.push([resistors[i], resistors[j]]);
        score.push(((rat-ratio) ** 2.0) ** 0.5);
      }
      else if (ratinv > lb && ratinv < ub) {

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
    var r1 = pair[0], r2 = pair[1], ratio;

    //if (ratio < 1.0) {
    //  r1 = smol;
    //  r2 = larg;
    //}
    //else {
    //  r1 = larg;
    //  r2 = smol;
    //}

    ratio = getratio(Number(r1), Number(r2));
    ratio = ratio.toFixed(3);

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

  clear_results();

  switch (situation) {
    case 1:
      expl.innerHTML  = "For a voltage divider:<br>";
      expl.innerHTML += "<img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/2/21/Resistive_divider2.svg/220px-Resistive_divider2.svg.png\" class=\"centre\" />";
      expl.innerHTML += "$$\\frac{V_{out}}{V_{in}} = \\frac{R_2}{R_1 + R_2}$$";
      expl.innerHTML += "Enter the <b>voltage ratio</b> below.";
      break;
    case 2:
      expl.innerHTML  = "For an inverting op-amp:<br>"
      expl.innerHTML += "<img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/4/41/Op-Amp_Inverting_Amplifier.svg/300px-Op-Amp_Inverting_Amplifier.svg.png\" class=\"centre\"/>";
      expl.innerHTML += "$$\\frac{V_{out}}{V_{in}} = -\\frac{R_f}{R_{in}} = -\\frac{R_2}{R_1}$$";
      expl.innerHTML += "Enter the <b>magnitude</b> of the desired <b>voltage ratio</b> below.";
      break;
    case 3:
      expl.innerHTML  = "For an inverting op-amp:<br>"
      expl.innerHTML += "<img src=\"https://upload.wikimedia.org/wikipedia/commons/thumb/4/44/Op-Amp_Non-Inverting_Amplifier.svg/300px-Op-Amp_Non-Inverting_Amplifier.svg.png\" class=\"centre\"/>";
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

function update_labels() {
  update_ratio_label();
  update_res_ex();
}
