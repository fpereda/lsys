#!/usr/bin/env bash

dosed() {
	if /usr/bin/sed --version &>/dev/null ; then
		/usr/bin/sed "$@"
		return $?
	elif gnused --version &>/dev/null ; then
		gnused "$@"
		return $?
	else
		echo "!!! No usable sed here." 1>&2
		exit 1
	fi
}

get_name() {
	dosed -n -e '/^Name: \(.*\)$/s--\1-p' $1
}

get_desc() {
	dosed -n -e '/^Description: \(.*\)$/s--\1-p' $1
}

get_code() {
	dosed -n -e '/^{$/,/^}$/p' $1
}

if [[ $# != 3 ]] ; then
	echo "Usage: $0 generated-template directory inputfile"
	exit 1
fi

generated=$1
dir=$2
inputfile=$3
outputfile=${inputfile%.in}

if [[ ! -d ${dir} ]] ; then
	echo "Error: '${dir}' is not a directory."
	exit 2
fi

if [[ ! -e ${inputfile} || -d ${inputfile} ]] ; then
	echo "Error: '${inputfile}' is not a regular file."
	exit 3
fi

dosed -e "/@GENERATED_FILE@/r ${generated}" \
	-e '/@GENERATED_FILE@/d' "${outputfile}" \
	"${inputfile}" > "${outputfile}"

for f in "${dir}"/* ; do
	key=${f##*/}
	func=${key//-/_}
	name=$(get_name "${f}")
	desc=$(get_desc "${f}")
	code=$(get_code "${f}")
	entry="{\"${name}\", \"${key}\", \"${desc}\", example_${func}},"
	funhead='void example_'${func}'(struct lsys_opts *o)'
	dosed -i -e "/@LSYSEXAMPLE_DEFS@/i ${funhead};" "${outputfile}"
	dosed -i -e "/@LSYSEXAMPLE_ENTRIES@/i ${entry}" "${outputfile}"
	dosed -i -e "/@LSYSEXAMPLE_FUNCTIONS@/i\
			${funhead}\n@LSYSEXAMPLE_FUNBODY@\n" \
			"${outputfile}"
	tmpfile=$(mktemp lsysXXXXXX)
	if [[ ! -w ${tmpfile} ]] ; then
		echo "Can't write to temp file (${tmpfile})"
		exit 4
	fi
	echo -e "${code}" > "${tmpfile}"
	dosed -i -e "/@LSYSEXAMPLE_FUNBODY@/r ${tmpfile}" \
			-e '/@LSYSEXAMPLE_FUNBODY@/d' "${outputfile}"
	rm -f "${tmpfile}"
done

dosed -i -e '/@LSYSEXAMPLE_\(DEFS\|ENTRIES\|FUNCTIONS\)@/d' "${outputfile}"
