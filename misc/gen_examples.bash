#!/bin/bash

get_name() {
	sed -n -e '/^Name: \(.*\)$/s--\1-p' $1
}

get_desc() {
	sed -n -e '/^Description: \(.*\)$/s--\1-p' $1
}

get_code() {
	sed -n -e '/^{$/,/^}$/p' $1
}

if [[ $# != 2 ]] ; then
	echo "Usage: $0 directory inputfile"
	exit 1
fi

dir=$1
inputfile=$2
outputfile=${inputfile%.in}

if [[ ! -d ${dir} ]] ; then
	echo "Error: '${dir}' is not a directory."
	exit 2
fi

if [[ ! -e ${inputfile} || -d ${inputfile} ]] ; then
	echo "Error: '${inputfile}' is not a regular file."
	exit 3
fi

sed -e '/@GENERATED_FILE@/r misc/generated-file.txt' \
	-e '/@GENERATED_FILE@/d' \
	${inputfile} > ${outputfile}

for f in ${dir}/* ; do
	key=${f##*/}
	func=${key//-/_}
	name=$(get_name ${f})
	desc=$(get_desc ${f})
	code=$(get_code ${f})
	entry="{\"${name}\", \"${key}\", \"${desc}\", example_${func}},"
	funhead='void example_'${func}'(struct lsys_opts *o)'
	sed -i -e "/@LSYSEXAMPLE_DEFS@/i ${funhead};" \
		${outputfile}
	sed -i -e "/@LSYSEXAMPLE_ENTRIES@/i ${entry}" \
		${outputfile}
	sed -i -e "/@LSYSEXAMPLE_FUNCTIONS@/i\
		${funhead}\n@LSYSEXAMPLE_FUNBODY@\n" \
		${outputfile}
	tmpfile=$(mktemp)
	echo -e "${code}" > ${tmpfile}
	sed -i -e "/@LSYSEXAMPLE_FUNBODY@/r ${tmpfile}" \
		-e '/@LSYSEXAMPLE_FUNBODY@/d' ${outputfile}
done

sed -i \
	-e '/@LSYSEXAMPLE_DEFS@/d' \
	-e '/@LSYSEXAMPLE_ENTRIES@/d' \
	-e '/@LSYSEXAMPLE_FUNCTIONS@/d' \
	${outputfile}
