const { execSync } = require("child_process");
const fs = require("fs");
const path = require("path");

// 1. Settings
const templateContent = `{{#sprites}}
// {{name}}
#define {{name}}Width {{width}}
#define {{name}}Height {{height}}
#define {{name}}Flags {{flagsHumanReadable}}
const uint8_t {{name}}[{{length}}] = { {{bytes}} };
{{/sprites}}`;

const templateFile = "sprite_template.mustache";
const imagePath = process.argv[2];

// 2. Validation
if (!imagePath) {
  console.error("Usage: node convert.js <path-to-image.png>");
  process.exit(1);
}

// 3. Create template file if it doesn't exist
if (!fs.existsSync(templateFile)) {
  console.log(`Creating ${templateFile}...`);
  fs.writeFileSync(templateFile, templateContent);
}

// 4. Run the conversion
try {
  console.log(`Converting ${imagePath}...`);

  // This runs the CLI command: w4 png2src --template ... <image>
  const output = execSync(
    `w4 png2src --template ${templateFile} "${imagePath}"`,
    {
      encoding: "utf-8",
    }
  );

  // 5. Output the result
  console.log("\n--- Generated C Code ---\n");
  console.log(output);

  // Optional: Save to a header file automatically
  // const headerFile = 'src/sprites.h';
  // fs.appendFileSync(headerFile, output + '\n');
  // console.log(`Appended to ${headerFile}`);
} catch (error) {
  console.error(
    "Error running w4 tool. Make sure wasm4 is installed (npm install -g wasm4)."
  );
  console.error(error.message);
}
